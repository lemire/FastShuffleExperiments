
#include "sillyreservoir.h"
#include "benchmark.h"
#include <algorithm>
#include <math.h>
#include <random>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCLUDEFLOAT

template <randfnc32 rfnc32> void ReservoirBenchmark32(size_t capacity, size_t size, bool verbose) {
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
  uint32_t *reservoir = (uint32_t *) malloc(size * sizeof(uint32_t));

#ifdef INCLUDESTDSHUFFLE
  UniformRandomBitGenerator32Struct<rfnc32> gen;

  BEST_TIME_NS(std::shuffle(reservoir, reservoir + size, gen),
               , repeat, size, verbose);
#endif

  BEST_TIME_NS(sillyreservoirsampling_go32<rfnc32>(reservoir, capacity, size),
               , repeat, size, verbose);
  BEST_TIME_NS(sillyreservoirsampling_java32<rfnc32>(reservoir, capacity, size),
               , repeat, size, verbose);

#ifdef INCLUDEFLOAT
  BEST_TIME_NS(sillyreservoirsampling_floatmult32<rfnc32>(reservoir, capacity, size),
               , repeat, size, verbose);

#endif

  BEST_TIME_NS(sillyreservoirsampling_nearlydivisionless32<rfnc32>(reservoir, capacity, size),
               , repeat, size, verbose);

  printf("        %d ", reservoir[0]);
  free(reservoir);
  printf("\n");
}

template <randfnc64 rfnc64> void ReservoirBenchmark64(size_t capacity, size_t size, bool verbose) {
  if (verbose) {
    printf(" %s\n", __PRETTY_FUNCTION__);
    printf("Shuffling arrays of size %zu \n", size);
    printf("Time reported in number of ns per array element.\n");
    printf("Tests assume that array is in cache as much as possible.\n");
  } else {
    printf("%zu ", size);
  }
  int repeat = 5;
  if (size < 1000000)
    repeat = 50;
  if (size > 10000000)
    repeat = 1;
  uint32_t *reservoir = (uint32_t *) malloc(size * sizeof(uint32_t));

#ifdef INCLUDESTDSHUFFLE
  UniformRandomBitGenerator64Struct<rfnc64> gen;
  BEST_TIME_NS(std::shuffle(reservoir, reservoir + size, gen),
               , repeat, size, verbose);
#endif

  BEST_TIME_NS(sillyreservoirsampling_go64<rfnc64>(reservoir, capacity, size),
               , repeat, size, verbose);


  BEST_TIME_NS(sillyreservoirsampling_java64<rfnc64>(reservoir, capacity, size),
               , repeat, size, verbose);

  BEST_TIME_NS(sillyreservoirsampling_floatmult64<rfnc64>(reservoir, capacity, size),
               , repeat, size, verbose);

#ifdef INCLUDEFLOAT
  BEST_TIME_NS(sillyreservoirsampling_nearlydivisionless64<rfnc64>(reservoir, capacity, size),
               , repeat, size, verbose);
#endif

  printf("        %d ", reservoir[0]);
  free(reservoir);
  printf("\n");

}

int main() {
  setseed(12345);

  printf("# We interleave 32-bit / 64-bit results. \n");
  printf("# First column uses an OpenBSD-like approach with two divisions per "
         "ranged random number.\n");
  printf("# Second column uses a Java-like approach with one or more divisions "
         "per ranged random number.\n");
  printf("# Third column uses an approach with nearly no division per ranged "
         "random number.\n");

  printf("# Time reported in number of ns per array element in a random "
         "shuffle.\n");
  size_t capacity = 10;
  for (size_t N = 10; N <= 1000 * 1000 * 1000; N *= 10) {
    ReservoirBenchmark32<lehmer64_32>(capacity, N, false);
    ReservoirBenchmark64<lehmer64>(capacity, N, false);
  }
  return 0;
}
