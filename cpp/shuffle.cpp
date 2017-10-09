
#include "shuffle.h"
#include "benchmark.h"
#include <algorithm>
#include <math.h>
#include <random>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Sorting stuff

*/

int qsort_compare_uint32_t(const void *a, const void *b) {
  return (*(uint32_t *)a - *(uint32_t *)b);
}

uint32_t *create_sorted_array(size_t length) {
  uint32_t *array = (uint32_t *)malloc(length * sizeof(uint32_t));
  for (size_t i = 0; i < length; i++)
    array[i] = (uint32_t)pcg32_random();
  qsort(array, length, sizeof(*array), qsort_compare_uint32_t);
  return array;
}

uint32_t *create_random_array(size_t count) {
  uint32_t *targets = (uint32_t *)malloc(count * sizeof(uint32_t));
  for (size_t i = 0; i < count; i++)
    targets[i] = (uint32_t)(pcg32_random() & 0x7FFFFFF);
  return targets;
}

// tries to put the array in cache
void array_cache_prefetch(uint32_t *B, int32_t length) {
  const int32_t CACHELINESIZE = 64; // 64 bytes per cache line
  for (int32_t k = 0; k < length; k += CACHELINESIZE / sizeof(uint32_t)) {
    __builtin_prefetch(B + k);
  }
}

int sortAndCompare(uint32_t *shuf, uint32_t *orig, uint32_t size) {
  qsort(shuf, size, sizeof(uint32_t), qsort_compare_uint32_t);
  qsort(orig, size, sizeof(uint32_t), qsort_compare_uint32_t);
  for (uint32_t k = 0; k < size; ++k)
    if (shuf[k] != orig[k]) {
      printf("[bug]\n");
      return -1;
    }
  return 0;
}
#define INCLUDEFLOAT

template <randfnc32 rfnc32> void ShuffleBenchmark32(size_t size, bool verbose) {
  if (verbose) {

    printf(" %s\n", __PRETTY_FUNCTION__);
    printf("Shuffling arrays of size %zu \n", size);
    printf("Time reported in number of cycles per array element.\n");
    printf("Tests assume that array is in cache as much as possible.\n");
  } else {
    printf("%zu ", size);
  }
  int repeat = 5;
  if (size < 1000000)
    repeat = 50;
  if (size > 10000000)
    repeat = 1;
  bool sortandcompare = size < 1000000;
  uint32_t *testvalues = create_random_array(size);
  uint32_t *pristinecopy = (uint32_t *)malloc(size * sizeof(uint32_t));
  memcpy(pristinecopy, testvalues, sizeof(uint32_t) * size);

#ifdef INCLUDESTDSHUFFLE
  UniformRandomBitGenerator32Struct<rfnc32> gen;

  // BEST_TIME(std::shuffle(testvalues, testvalues + size, gen),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(std::shuffle(testvalues, testvalues + size, gen),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
#endif

  // BEST_TIME(shuffle_go32<rfnc32>(testvalues, size),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(shuffle_go32<rfnc32>(testvalues, size),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;

  // BEST_TIME(shuffle_java32<rfnc32>(testvalues, size),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(shuffle_java32<rfnc32>(testvalues, size),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;

#ifdef INCLUDEFLOAT
  // BEST_TIME(shuffle_floatmult32<rfnc32>(testvalues, size),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(shuffle_floatmult32<rfnc32>(testvalues, size),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
#endif

  // BEST_TIME(shuffle_nearlydivisionless32<rfnc32>(testvalues, size),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(shuffle_nearlydivisionless32<rfnc32>(testvalues, size),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;

  free(testvalues);
  free(pristinecopy);
  printf("\n");
}

template <randfnc64 rfnc64> void ShuffleBenchmark64(size_t size, bool verbose) {
  if (verbose) {
    printf(" %s\n", __PRETTY_FUNCTION__);
    printf("Shuffling arrays of size %zu \n", size);
    printf("Time reported in number of cycles per array element.\n");
    printf("Tests assume that array is in cache as much as possible.\n");
  } else {
    printf("%zu ", size);
  }
  int repeat = 5;
  if (size < 1000000)
    repeat = 50;
  if (size > 10000000)
    repeat = 1;
  bool sortandcompare = size < 1000000;
  uint32_t *testvalues = create_random_array(size);
  uint32_t *pristinecopy = (uint32_t *)malloc(size * sizeof(uint32_t));
  memcpy(pristinecopy, testvalues, sizeof(uint32_t) * size);

#ifdef INCLUDESTDSHUFFLE
  UniformRandomBitGenerator64Struct<rfnc64> gen;
  // BEST_TIME(std::shuffle(testvalues, testvalues + size, gen),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(std::shuffle(testvalues, testvalues + size, gen),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
#endif

  // BEST_TIME(shuffle_go64<rfnc64>(testvalues, size),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(shuffle_go64<rfnc64>(testvalues, size),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;

  // BEST_TIME(shuffle_java64<rfnc64>(testvalues, size),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(shuffle_java64<rfnc64>(testvalues, size),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;

  // BEST_TIME(shuffle_floatmult64<rfnc64>(testvalues, size),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(shuffle_floatmult64<rfnc64>(testvalues, size),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
#ifdef INCLUDEFLOAT
  // BEST_TIME(shuffle_nearlydivisionless64<rfnc64>(testvalues, size),
  //          array_cache_prefetch(testvalues, size), repeat, size);
  BEST_TIME_NS(shuffle_nearlydivisionless64<rfnc64>(testvalues, size),
               array_cache_prefetch(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
#endif

  free(testvalues);
  free(pristinecopy);
  printf("\n");
}

bool alltrue(bool *possibilities, size_t N) {
  for (size_t i = 0; i < N; i++)
    if (!possibilities[i])
      return false;
  return true;
}

template <randfnc32 rfnc32> void test() {
  const int N = 3;
  uint32_t testvalues[3];
  testvalues[0] = 0;
  testvalues[1] = 1;
  testvalues[2] = 2;

  // there is a smarter way to do this:

  {
    bool possibilities[6] = {0};
    while (!alltrue(possibilities, 6)) {
      shuffle_go32<rfnc32>(testvalues, N);
      uint32_t c = testvalues[1];
      for (uint32_t z = 0; z < 3; z++) {
        if (z == testvalues[1])
          continue;
        if (z == testvalues[0])
          break;
        c += 3;
      }
      possibilities[c] = true;
    }
  }

  {
    bool possibilities[6] = {0};
    while (!alltrue(possibilities, 6)) {
      shuffle_java32<rfnc32>(testvalues, N);
      uint32_t c = testvalues[1];
      for (uint32_t z = 0; z < 3; z++) {
        if (z == testvalues[1])
          continue;
        if (z == testvalues[0])
          break;
        c += 3;
      }
      possibilities[c] = true;
    }
  }

  {
    bool possibilities[6] = {0};
    while (!alltrue(possibilities, 6)) {
      shuffle_floatmult<rfnc32>(testvalues, N);
      uint32_t c = testvalues[1];
      for (uint32_t z = 0; z < 3; z++) {
        if (z == testvalues[1])
          continue;
        if (z == testvalues[0])
          break;
        c += 3;
      }
      possibilities[c] = true;
    }
  }

  {
    bool possibilities[6] = {0};
    while (!alltrue(possibilities, 6)) {
      shuffle_nearlydivisionless32<rfnc32>(testvalues, N);
      uint32_t c = testvalues[1];
      for (uint32_t z = 0; z < 3; z++) {
        if (z == testvalues[1])
          continue;
        if (z == testvalues[0])
          break;
        c += 3;
      }
      possibilities[c] = true;
    }
  }
}

int main() {
  setseed(12345);

  test<pcg32_random>();
  for (size_t N = 10; N <= 1000 * 1000 * 1000; N *= 10) {
    ShuffleBenchmark32<lehmer64_32>(N, false);
    // ShuffleBenchmark32<pcg32_random>(N);
    // ShuffleBenchmark32<xorshift128plus_32>(N);
    // ShuffleBenchmark32<splitmix64_cast32>(N);
    // ShuffleBenchmark32<twister>(N);
    ShuffleBenchmark64<lehmer64>(N, false);
  }
  return 0;
}
