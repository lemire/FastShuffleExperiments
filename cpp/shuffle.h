#include "rangedrand.h"
#include <stdint.h>
#define BUFFERSIZE32 256
#define BUFFERSIZE64 256

void shuffle_precomputed(uint32_t *storage, uint32_t size, const uint32_t *precomputedvals) {
  uint32_t i;
  for (i = size; i > 1; i--) {
    uint32_t nextpos = precomputedvals[i];
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// good old Fisher-Yates shuffle, shuffling an array of integers, uses java-like
// ranged rng
template <randfnc32 UniformRandomBitGenerator>
void shuffle_java32(uint32_t *storage, uint32_t size) {
  uint32_t i;
  for (i = size; i > 1; i--) {
    uint32_t nextpos = java_random_bounded32<UniformRandomBitGenerator>(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}
template <randfnc32 UniformRandomBitGenerator, size_t block = BUFFERSIZE32>
void buf_shuffle_java32(uint32_t *storage, uint32_t size) {
  uint32_t i = size;
  uint32_t buffer[block];
  for ( ; i > block + 1; i-=block) {
    for(size_t k = 0; k < block; k++)
        buffer[k] = java_random_bounded32<UniformRandomBitGenerator>(i - k);
    for(size_t k = 0; k < block; k++) {
        uint32_t nextpos = buffer[k];
        uint32_t tmp = storage[i - 1 - k];
        uint32_t val = storage[nextpos];
        storage[i - 1 - k ] = val;
        storage[nextpos] = tmp;
    }
  }
  for ( ; i > 1; i--) {
    uint32_t nextpos = java_random_bounded32<UniformRandomBitGenerator>(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}
// good old Fisher-Yates shuffle, shuffling an array of integers, uses java-like
// ranged rng
template <randfnc64 UniformRandomBitGenerator>
void shuffle_java64(uint32_t *storage, uint64_t size) {
  uint64_t i;
  for (i = size; i > 1; i--) {
    uint64_t nextpos = java_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

template <randfnc64 UniformRandomBitGenerator, size_t block = BUFFERSIZE64>
void buf_shuffle_java64(uint32_t *storage, uint64_t size) {
  uint64_t i = size;
  uint64_t buffer[block];
  for ( ; i > block + 1; i-=block) {
    for(size_t k = 0; k < block; k++)
        buffer[k] = java_random_bounded64<UniformRandomBitGenerator>(i - k);
    for(size_t k = 0; k < block; k++) {
        uint64_t nextpos = buffer[k];
        uint64_t tmp = storage[i - 1 - k];
        uint64_t val = storage[nextpos];
        storage[i - 1 - k ] = val;
        storage[nextpos] = tmp;
    }
  }
  for (; i > 1; i--) {
    uint64_t nextpos = java_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// good old Fisher-Yates shuffle, shuffling an array of integers
template <randfnc32 UniformRandomBitGenerator>
void shuffle_floatmult32(uint32_t *storage, uint32_t size) {
  uint32_t i;
  for (i = size; i > 1; i--) {
    uint32_t nextpos = floatmult_random_bounded32<UniformRandomBitGenerator>(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// good old Fisher-Yates shuffle, shuffling an array of integers
template <randfnc32 UniformRandomBitGenerator, size_t block = BUFFERSIZE32>
void buf_shuffle_floatmult32(uint32_t *storage, uint32_t size) {
  uint32_t i = size;
  uint32_t buffer[block];
  for ( ; i > block + 1; i-=block) {
    for(size_t k = 0; k < block; k++)
        buffer[k] = floatmult_random_bounded32<UniformRandomBitGenerator>(i - k);
    for(size_t k = 0; k < block; k++) {
        uint32_t nextpos = buffer[k];
        uint32_t tmp = storage[i - 1 - k];
        uint32_t val = storage[nextpos];
        storage[i - 1 - k ] = val;
        storage[nextpos] = tmp;
    }
  }
  for (; i > 1; i--) {
    uint32_t nextpos = floatmult_random_bounded32<UniformRandomBitGenerator>(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// good old Fisher-Yates shuffle, shuffling an array of integers
template <randfnc64 UniformRandomBitGenerator>
void shuffle_floatmult64(uint32_t *storage, uint64_t size) {
  uint64_t i;
  for (i = size; i > 1; i--) {
    uint64_t nextpos = floatmult_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// good old Fisher-Yates shuffle, shuffling an array of integers
template <randfnc64 UniformRandomBitGenerator, size_t block = BUFFERSIZE64>
void buf_shuffle_floatmult64(uint32_t *storage, uint64_t size) {
  uint64_t i = size;
  uint64_t buffer[block];
  for ( ; i > block + 1; i-=block) {
    for(size_t k = 0; k < block; k++)
        buffer[k] = floatmult_random_bounded64<UniformRandomBitGenerator>(i - k);
    for(size_t k = 0; k < block; k++) {
        uint64_t nextpos = buffer[k];
        uint64_t tmp = storage[i - 1 - k];
        uint64_t val = storage[nextpos];
        storage[i - 1 - k ] = val;
        storage[nextpos] = tmp;
    }
  }
  for (; i > 1; i--) {
    uint64_t nextpos = floatmult_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// good old Fisher-Yates shuffle, shuffling an array of integers, uses go-like
// ranged rng
template <randfnc32 UniformRandomBitGenerator>
void shuffle_go32(uint32_t *storage, uint32_t size) {
  uint32_t i;
  for (i = size; i > 1; i--) {
    uint32_t nextpos = go_random_bounded32<UniformRandomBitGenerator>(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

template <randfnc32 UniformRandomBitGenerator, size_t block = BUFFERSIZE32>
void buf_shuffle_go32(uint32_t *storage, uint32_t size) {
  uint32_t i = size;
  uint32_t buffer[block];
  for ( ; i > block + 1; i-=block) {
    for(size_t k = 0; k < block; k++)
        buffer[k] = go_random_bounded32<UniformRandomBitGenerator>(i - k);
    for(size_t k = 0; k < block; k++) {
        uint32_t nextpos = buffer[k];
        uint32_t tmp = storage[i - 1 - k];
        uint32_t val = storage[nextpos];
        storage[i - 1 - k ] = val;
        storage[nextpos] = tmp;
    }
  }
  for (; i > 1; i--) {
    uint32_t nextpos = go_random_bounded32<UniformRandomBitGenerator>(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

template <randfnc64 UniformRandomBitGenerator>
void shuffle_go64(uint32_t *storage, uint64_t size) {
  uint64_t i;
  for (i = size; i > 1; i--) {
    uint64_t nextpos = go_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

template <randfnc64 UniformRandomBitGenerator, size_t block = BUFFERSIZE64>
void buf_shuffle_go64(uint32_t *storage, uint64_t size) {
  uint64_t i = size;
  uint64_t buffer[block];
  for ( ; i > block + 1; i-=block) {
    for(size_t k = 0; k < block; k++)
        buffer[k] = go_random_bounded64<UniformRandomBitGenerator>(i - k);
    for(size_t k = 0; k < block; k++) {
        uint64_t nextpos = buffer[k];
        uint64_t tmp = storage[i - 1 - k];
        uint64_t val = storage[nextpos];
        storage[i - 1 - k ] = val;
        storage[nextpos] = tmp;
    }
  }
  for (; i > 1; i--) {
    uint64_t nextpos = go_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// good old Fisher-Yates shuffle, shuffling an array of integers, without
// division
template <randfnc32 UniformRandomBitGenerator>
void shuffle_nearlydivisionless32(uint32_t *storage, uint32_t size) {
  uint32_t i;
  for (i = size; i > 1; i--) {
    uint32_t nextpos =
        random_bounded_nearlydivisionless32<UniformRandomBitGenerator>(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

template <randfnc32 UniformRandomBitGenerator, size_t block = BUFFERSIZE32>
void buf_shuffle_nearlydivisionless32(uint32_t *storage, uint32_t size) {
  uint32_t i = size;
  uint32_t buffer[block];
  for ( ; i > block + 1; i-=block) {
    for(size_t k = 0; k < block; k++)
        buffer[k] = random_bounded_nearlydivisionless32<UniformRandomBitGenerator>(i - k);
    for(size_t k = 0; k < block; k++) {
        uint32_t nextpos = buffer[k];
        uint32_t tmp = storage[i - 1 - k];
        uint32_t val = storage[nextpos];
        storage[i - 1 - k ] = val;
        storage[nextpos] = tmp;
    }
  }
  for (; i > 1; i--) {
    uint32_t nextpos =
        random_bounded_nearlydivisionless32<UniformRandomBitGenerator>(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}
// good old Fisher-Yates shuffle, shuffling an array of integers, without
// division
template <randfnc64 UniformRandomBitGenerator>
void shuffle_nearlydivisionless64(uint32_t *storage, uint64_t size) {
  uint64_t i;
  for (i = size; i > 1; i--) {
    uint64_t nextpos =
        random_bounded_nearlydivisionless64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

template <randfnc64 UniformRandomBitGenerator, size_t block = BUFFERSIZE64>
void buf_shuffle_nearlydivisionless64(uint32_t *storage, uint64_t size) {
  uint64_t i = size;
  uint64_t buffer[block];
  for ( ; i > block + 1; i-=block) {
    for(size_t k = 0; k < block; k++)
        buffer[k] = random_bounded_nearlydivisionless64<UniformRandomBitGenerator>(i - k);
    for(size_t k = 0; k < block; k++) {
        uint64_t nextpos = buffer[k];
        uint64_t tmp = storage[i - 1 - k];
        uint64_t val = storage[nextpos];
        storage[i - 1 - k ] = val;
        storage[nextpos] = tmp;
    }
  }
  for (; i > 1; i--) {
    uint64_t nextpos =
        random_bounded_nearlydivisionless64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}
