#pragma once
// http://xoroshiro.di.unimi.it/splitmix64.c

#include "types.hpp"

class splitmix64
{
private:
  uint64 x;

  inline uint64 next() 
  {
    uint64 z = (x += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
  }

public:
  // splitmix64()
  //   : x(std::random_device()())
  // {}
  splitmix64(uint64 seed)
    : x(seed)
  {}

  inline uint64 operator()()
  {
    return next();
  }

  inline uint64 get()
  {
    return next();
  }

  void setSeed(uint64 seed)
  {
    x = seed;
  }
};
