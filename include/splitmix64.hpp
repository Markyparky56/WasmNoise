#pragma once
// http://xoroshiro.di.unimi.it/splitmix64.c

#include <cstdint>
#include <random>

using std::uint64_t;

class splitmix64
{
private:
  uint64_t x;

  inline uint64_t next() 
  {
    uint64_t z = (x += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
  }

public:
  splitmix64()
    : x(std::random_device()())
  {}
  splitmix64(uint64_t seed)
    : x(seed)
  {}

  inline uint64_t operator()()
  {
    return next();
  }

  inline uint64_t get()
  {
    return next();
  }

  void setSeed(uint64_t seed)
  {
    x = seed;
  }
};
