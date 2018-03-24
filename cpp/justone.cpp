
#include "shuffle.h"
#include "benchmark.h"
#include <algorithm>
#include <math.h>
#include <random>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t *create_array(size_t count) {
  uint32_t *targets = (uint32_t *)malloc(count * sizeof(uint32_t));
  for (size_t i = 0; i < count; i++)
    targets[i] = i; 
  return targets;
}

template <randfnc32 rfnc32> void ShuffleBenchmark32(size_t size, bool verbose) {
  if (verbose) {

    printf(" %s\n", __PRETTY_FUNCTION__);
    printf("Shuffling arrays of size %zu \n", size);
    printf("Time reported in number of cycles per array element.\n");
    printf("Tests assume that array is in cache as much as possible.\n");
  } else {
    printf("%zu or %f MB ", size, size * 4.0/(1024.0 * 1024.0));
  }
  printf("\n creating array...\n");
  uint32_t *testvalues = create_array(size);
  printf("created array\n");
  for (int k = 0; k < 10; k ++) {
    shuffle_go32<rfnc32>(testvalues, size);
    printf("One shuffle complete\n");
  }
  printf("%u \n", testvalues[0]);
  free(testvalues);
}



int main(int argc, char** argv) {
  if(argc > 1) {
            printf("unknown flag\n");
            return EXIT_FAILURE;
  }
  setseed(12345);
  ShuffleBenchmark32<lehmer64_32>(100 * 1000 * 1000, false);
  return 0;
}
