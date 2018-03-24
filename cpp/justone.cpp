
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

void ShuffleBenchmark(size_t size, int mode, bool verbose) {
  if (verbose) {

    printf(" %s\n", __PRETTY_FUNCTION__);
    printf("Shuffling arrays of size %zu \n", size);
    printf("Time reported in number of cycles per array element.\n");
    printf("Tests assume that array is in cache as much as possible.\n");
  } else {
    printf("%zu or %f MB ", size, size * 4.0/(1024.0 * 1024.0));
  }
  uint32_t *testvalues = create_array(size);
  switch(mode) {
    case 1:
    printf("shuffle_go32\n");
    shuffle_go32<lehmer64_32>(testvalues, size);
    break;
    case 2:
    printf("shuffle_java32\n");
    shuffle_java32<lehmer64_32>(testvalues, size);
    break;
    case 3:
    printf("shuffle_nearlydivisionless32\n");
    shuffle_nearlydivisionless32<lehmer64_32>(testvalues, size);
    break;
    case 4:
    printf("shuffle_floatmult32\n");
    shuffle_floatmult32<lehmer64_32>(testvalues, size);
    break;
    case 5:
    printf("shuffle_go64\n");
    shuffle_go64<lehmer64>(testvalues, size);
    break;
    case 6:
    printf("shuffle_java64\n");
    shuffle_java64<lehmer64>(testvalues, size);
    break;
    case 7:
    printf("shuffle_nearlydivisionless64\n");
    shuffle_nearlydivisionless64<lehmer64>(testvalues, size);
    break;
    case 8:
    printf("shuffle_floatmult64\n");
    shuffle_floatmult64<lehmer64>(testvalues, size);
    break;
    default:
    break;
  }

  printf("%u \n", testvalues[0]);
  free(testvalues);
}



int main(int argc, char** argv) {
  int mode = 100;
  if(argc > 1) {
            mode = atoi(argv[1]);
            //return EXIT_FAILURE;
  }
  setseed(12345);
  ShuffleBenchmark(100 * 1000 * 1000, mode, false);
  return 0;
}
