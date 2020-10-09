# FastShuffleExperiments
How fast can we shuffle values?

The [nearly divisionless technique](https://arxiv.org/abs/1805.10941) benchmarked in this repository has been widely adopted:
  * by the [standard C++ Linux library (GNU libstdc++)](https://gcc.gnu.org/git/?p=gcc.git;a=blobdiff;f=libstdc%2B%2B-v3/include/bits/uniform_int_dist.h;h=ecb8574864aee10b9ea164379fffef27c7bdb0df;hp=6e1e3d5fc5fe8f7f22e62a85b35dc8bfa4743372;hb=98c37d3bacbb2f8bbbe56ed53a9547d3be01b66b;hpb=6ce2cb116af6e0965ff0dd69e7fd1925cf5dc68c) to accelerate the std::uniform_int_distribution function,
 * by [Google's Abseil C++ Common Libraries](https://github.com/abseil/abseil-cpp),
 * by [the Swift standard library](https://github.com/apple/swift/pull/25286),
 * by the [Go language](https://github.com/golang/go/commit/a2dfe5d278eae0864397a046a8206342a426d2bd),
 * by the  [Julia language](https://github.com/JuliaLang/julia/pull/29240),
 * by the [Zig language](https://github.com/ziglang/zig/blob/98183e47436699f6e5eab200061c46eec342806e/std/rand.zig#L74-L118),
 * and by [Numpy](https://github.com/numpy/numpy/blob/6420e7f528a6c42422966544e453bdb2805ff620/numpy/random/generator.pyx) (Python).
 
 # Requirements

We assume that you have a Linux-like system.

To reproduce the figures from the ACM Transactions on Modeling and Computer Simulation article, please go to the TOMACS_RCR subdirectory and consult the README.md file for instructions. 

Usage:

```
cd cpp
make
./shuffle
```
# Why is it faster?

The core idea is that you can achieve faster shuffling by avoiding division. We 
generate division when trying to map random numbers to an interval. Let us compare
these three functions:

```C++
// returns value in [0,s)
// random64 is a function returning random 64-bit words
uint64_t openbsd(uint64_t s, uint64_t (*random64)(void)) {
  uint64_t t = (-s) % s;
  uint64_t x;
  do {
    x = random64();
  } while (x < t);
  return x % s;
}

// returns value in [0,s)
// random64 is a function returning random 64-bit words
uint64_t java(uint64_t s, uint64_t (*random64)(void)) {
  uint64_t x = random64();
  uint64_t r = x % s;
  while (x - r >
         UINT64_MAX - s + 1) { 
    x = random64();
    r = x % s;
  }
  return r;
}

// returns value in [0,s)
// random64 is a function returning random 64-bit words
uint64_t nearlydivisionless(uint64_t s, uint64_t (*random64)(void)) {
  uint64_t x = random64();
  __uint128_t m = (__uint128_t) x * (__uint128_t) s;
  uint64_t l = (uint64_t) m;
  if (l < s) {
   uint64_t t = -s % s;
    while (l < t) {
      x = random64();
      m = (__uint128_t) x * (__uint128_t) s;
      l = (uint64_t) m;
    }
  }
  return m >> 64; 
}
```

The first one always generates two divisions, the second one usually generates one division whereas the last one rarely requires a division at all.

## Reference

* Daniel Lemire, [Fast Random Integer Generation in an Interval](https://arxiv.org/abs/1805.10941), ACM Transactions on Modeling and Computer Simulation Volume 29 Issue 1, February 2019
