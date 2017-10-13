#include "chisqr.h"
#include "rangedrand.h"
#include <cassert>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

typedef uint32_t (*rangedfnc32)(uint32_t);
typedef uint64_t (*rangedfnc64)(uint64_t);

template <rangedfnc32 fnc>
bool isuniform32(size_t N, double &ret, bool verbose = true,
                 double tol = 0.01) {
  std::vector<double> counts(N);
  size_t volume = 10000 * N;
  for (size_t i = 0; i < volume; i++) {
    counts[fnc(N)] += 1.0;
  }
  double expected = (double)volume / N;
  double sum = 0;
  for (size_t i = 0; i < N; i++) {
    double x = counts[i] - expected;
    sum += x * x / expected;
  }
  ret = chisqr(N - 1, sum);
  if (ret < tol) {
    if (verbose)
      printf(" %s fails with %f [%f] N = %zu \n", __PRETTY_FUNCTION__, ret, sum,
             N);
  }
  return ret >= tol;
}

template <rangedfnc64 fnc>
bool isuniform64(size_t N, double &ret, bool verbose = true,
                 double tol = 0.01) {
  std::vector<double> counts(N);
  size_t volume = 10000 * N;
  for (size_t i = 0; i < volume; i++) {
    counts[fnc(N)] += 1.0;
  }
  double expected = (double)volume / N;
  double sum = 0;
  for (size_t i = 0; i < N; i++) {
    double x = counts[i] - expected;
    sum += x * x / expected;
  }
  ret = chisqr(N - 1, sum);
  if (ret < tol) {
    if (verbose)
      printf(" %s fails with %f [%f] N = %zu \n", __PRETTY_FUNCTION__, ret, sum,
             N);
  }
  return ret >= tol;
}

uint32_t bad(uint32_t x) { return x - 1; }

uint32_t badbutbetter(uint32_t x) { return rand() % (x - 1); }
uint32_t notgreat(uint32_t x) { return rand() % x; }

int main() {
  setseed(1234);
  size_t success = 0;
  size_t total = 0;
  for (size_t N = 2; N <= 300; N += 1) {
    if (chisqr(N - 1, 0) < 1.0)
      printf("N = %zu \n", N);
    assert(chisqr(N - 1, 0) == 1);
    double ret = 0;
    for (size_t repeat = 0; repeat < 5; repeat++) {
      assert(!isuniform32<bad>(N, ret, false));
      assert(!isuniform32<badbutbetter>(N, ret, false));
      bool randok = isuniform32<notgreat>(N, ret, false);
      if (!randok)
        printf("rand mod %zu : fails\n", N);
      total += 12;
      success += (isuniform32<random_bounded32<lehmer64_32>>(N, ret));
      success += (isuniform32<java_random_bounded32<lehmer64_32>>(N, ret));
      success += (isuniform32<go_random_bounded32<lehmer64_32>>(N, ret));
      success += (isuniform32<random_bounded_nearlydivisionless32<lehmer64_32>>(
          N, ret));
      success += (isuniform32<floatmult_random_bounded32<lehmer64_32>>(N, ret));
      success += (isuniform32<naive_random_bounded32<lehmer64_32>>(N, ret));
      success += (isuniform64<random_bounded64<lehmer64>>(N, ret));
      success += (isuniform64<java_random_bounded64<lehmer64>>(N, ret));
      success += (isuniform64<go_random_bounded64<lehmer64>>(N, ret));
      success +=
          (isuniform64<random_bounded_nearlydivisionless64<lehmer64>>(N, ret));
      success += (isuniform64<floatmult_random_bounded64<lehmer64>>(N, ret));
      success += (isuniform64<naive_random_bounded64<lehmer64>>(N, ret));
    }
  }
  if (total - success < 0.05 * total) {
    printf("Results look good, successes = %zu out of %zu. Functions pass "
           "sanity check. \n",
           success, total);
  } else {
    printf("Results are a problem. Report an issue. \n");
    return -1;
  }
}
