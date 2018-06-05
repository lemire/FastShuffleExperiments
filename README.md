# FastShuffleExperiments
How fast can we shuffle values?

We assume that you have a Linux-like system.

To reproduce the figures from the ACM Transactions on Modeling and Computer Simulation article, please go to the TOMACS_RCR subdirectory and consult the README.md file for instructions. 

Usage:

```
cd cpp
make
./shuffle
```

The core idea is that you can achieve faster shuffling by avoiding division. We 
generate division when trying to map random numbers to an interval. Let us compare
these three functions:

```
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

See:

* Daniel Lemire, [Fast Random Integer Generation in an Interval](https://arxiv.org/abs/1805.10941), ACM Transactions on Modeling and Computer Simulation (to appear)
