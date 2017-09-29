#pragma once
// http://xoroshiro.di.unimi.it/splitmix64.c

#include <cstdint>
#include <random>

using std::uint32_t;

class splitmix32
{
private:
  uint32_t x;

  inline uint32_t next() 
  {
    uint32_t z = (x += UINT32_C(0x9E3779B9));
    z = (z ^ (z >> 30)) * UINT32_C(0xBF58476D);
    z = (z ^ (z >> 27)) * UINT32_C(0x94D049BB);
    return z ^ (z >> 31);
  }

public:
  splitmix32()
    : x(std::random_device()())
  {}
  splitmix32(uint32_t seed)
    : x(seed)
  {}

  inline uint32_t operator()()
  {
    return next();
  }

  inline uint32_t get()
  {
    return next();
  }

  void setSeed(uint32_t seed)
  {
    x = seed;
  }
};
