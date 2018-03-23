#include "rangedrand.h"
#include <stdint.h>
// we implement reservoir sampling over the integers from 0 to size (excl.)


template <randfnc32 UniformRandomBitGenerator>
void sillyreservoirsampling_java32(uint32_t *storage, uint32_t capacity, uint32_t size) {
  uint32_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint32_t nextpos = java_random_bounded32<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc64 UniformRandomBitGenerator>
void sillyreservoirsampling_java64(uint32_t *storage, uint32_t capacity, uint64_t size) {
  uint64_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint64_t nextpos = java_random_bounded64<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc32 UniformRandomBitGenerator>
void sillyreservoirsampling_floatmult32(uint32_t *storage, uint32_t capacity, uint32_t size) {
  uint32_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint32_t nextpos = floatmult_random_bounded32<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc64 UniformRandomBitGenerator>
void sillyreservoirsampling_floatmult64(uint32_t *storage, uint32_t capacity, uint64_t size) {
  uint64_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint64_t nextpos = floatmult_random_bounded64<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc32 UniformRandomBitGenerator>
void sillyreservoirsampling_floatmult(uint32_t *storage, uint32_t capacity, uint32_t size) {
  uint32_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint32_t nextpos = floatmult_random_bounded32<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc32 UniformRandomBitGenerator>
void sillyreservoirsampling_go32(uint32_t *storage, uint32_t capacity, uint32_t size) {
  uint32_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint32_t nextpos = go_random_bounded32<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc64 UniformRandomBitGenerator>
void sillyreservoirsampling_go64(uint32_t *storage, uint32_t capacity, uint64_t size) {
  uint64_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint64_t nextpos = go_random_bounded64<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc32 UniformRandomBitGenerator>
void sillyreservoirsampling_go64(uint32_t *storage, uint32_t capacity, uint64_t size) {
  uint64_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint64_t nextpos = go_random_bounded64<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc32 UniformRandomBitGenerator>
void sillyreservoirsampling_nearlydivisionless32(uint32_t *storage, uint32_t capacity, uint32_t size) {
  uint32_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint32_t nextpos =
        random_bounded_nearlydivisionless32<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}

template <randfnc64 UniformRandomBitGenerator>
void sillyreservoirsampling_nearlydivisionless64(uint32_t *storage, uint32_t capacity, uint64_t size) {
  uint64_t i;
  for (i = 0; i < capacity; i++) {
    storage[i] = i;
  }
  for (i = capacity; i < size; i++) {
    uint64_t nextpos =
        random_bounded_nearlydivisionless64<UniformRandomBitGenerator>(i);
    if(nextpos < capacity) {
      storage[nextpos] = i;
    }
  }
}
