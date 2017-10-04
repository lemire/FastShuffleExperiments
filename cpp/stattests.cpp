#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include "chisqr.h"
#include "rangedrand.h"
#include <cassert>


typedef uint32_t (*rangedfnc32)(uint32_t);
typedef uint64_t (*rangedfnc64)(uint64_t);

template<rangedfnc32 fnc>
bool isuniform32(size_t N, double tol = 0.99) {
  std::vector<double> counts (N);
  size_t volume = 1000 * N;
  for(size_t i = 0; i < volume; i++) {
    counts[fnc(N)] += 1.0;
  }
  long double expected = volume * (long double)1.0 / N;
  long double sum = 0;
  for(size_t i = 0; i < N; i++) {
    long double x = (long double)counts[i] - expected;
    sum += x * x / expected;
  }
  double ret = chisqr(N - 1, sum);
  printf("%f \n", ret);
  return ret >= tol;
}

template<rangedfnc64 fnc>
bool isuniform64(size_t N, double tol = 0.99) {
  std::vector<double> counts (N);
  size_t volume = 1000 * N;
  for(size_t i = 0; i < volume; i++) {
    counts[fnc(N)] += 1.0;
  }
  long double expected = volume * (long double) 1.0 / N;
  long double sum = 0;
  for(size_t i = 0; i < N; i++) {
    long double x = counts[i] - expected;
    sum += x * x / expected;
  }
  double ret = chisqr(N - 1, sum);
  printf("%f \n", ret);

  return ret >= tol;
}


uint32_t bad(uint32_t x) {return x-1;}

uint32_t badbutbetter(uint32_t x) {return rand() % (x - 1);}
uint32_t notgreat(uint32_t x) {return rand() % x;}

int main() {
  setseed(1234);
  size_t N = 1000;
  //for( N = 500; N <= 50000; N*=10) {
    printf("range = %zu\n",N);
  assert(!isuniform32<bad >(N));
  assert(!isuniform32<badbutbetter >(N));
  printf("rand mod : %s\n", isuniform32<notgreat >(N) ? "passes" : "fails");

  assert(isuniform32<random_bounded32<lehmer64_32> >(N));
  assert(isuniform32<java_random_bounded32<lehmer64_32> >(N));
  assert(isuniform32<go_random_bounded32<lehmer64_32> >(N));
  assert(isuniform32<random_bounded_nearlydivisionless32<lehmer64_32> >(N));
  assert(isuniform32<floatmult_random_bounded32<lehmer64_32> >(N));
  assert(isuniform32<naive_random_bounded32<lehmer64_32> >(N));
  printf("\n");
  assert(isuniform64<random_bounded64<lehmer64> >(N));
  assert(isuniform64<java_random_bounded64<lehmer64> >(N));
  assert(isuniform64<go_random_bounded64<lehmer64> >(N));
  assert(isuniform64<random_bounded_nearlydivisionless64<lehmer64> >(N));
  assert(isuniform64<floatmult_random_bounded64<lehmer64> >(N));
  assert(isuniform64<naive_random_bounded64<lehmer64> >(N));
//}
  printf("Results look good. Functions pass sanity check. \n");

}
