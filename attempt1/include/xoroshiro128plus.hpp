#pragma once
// xoroshiro128plus class, converted from xoroshift128plus.c 
// by David Blackman & Sebastino Vigna
// Original file can be found here: http://xoroshiro.di.unimi.it/xoroshiro128plus.c
// Conversion by Mark A. Ropper

#include <cstdint>
#include <random>
#include "splitmix64.hpp"
#include <array>
#include <vector>
#include <utility> // std::swap
#include <random>

using std::uint64_t;

class xoroshiro128plus
{
private:
  uint64_t s[2];

  static inline uint64_t rotl(const uint64_t x, int k)
  {
    return (x << k) | (x >> (64 - k));
  }

  inline uint64_t next()
  {
    const uint64_t s0 = s[0];
    uint64_t s1 = s[1];
    const uint64_t result = s0 + s1;

    s1 ^= s0;
    s[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
    s[1] = rotl(s1, 36);

    return result;
  }

public:
  typedef uint64_t result_type; // Attempt to interface with the STL's uniform distributions
  static constexpr uint64_t min() { return std::numeric_limits<uint64_t>::min(); }
  static constexpr uint64_t max() { return std::numeric_limits<uint64_t>::max(); }

  xoroshiro128plus()
    : s{ std::random_device()(), std::random_device()() }
  {}
  xoroshiro128plus(uint64_t state[2])
    : s{ state[0], state[1]}
  {}
  // Recommended setup from xoroshiro128plus.c
  xoroshiro128plus(uint64_t seed)
  {
    setSeed(seed);
  }

  inline uint64_t operator()()
  {
    return next();
  }

  inline uint64_t get()
  {
    return next();
  }

  // This is the jump function for the generator. It is equivalent to
  // 2^64 calls to next(), it can be used to generate 2^64 non-overlapping
  // subsequences for parallel computations
  void jump()
  {
    static const uint64_t JUMP[] = { 0xbeac0467eba5facb, 0xd86b048b86aa9922 };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    for (int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
    {
      for (int b = 0; b < 64; b++)
      {
        if (JUMP[i] & UINT64_C(1) << b)
        {
          s0 ^= s[0];
          s1 ^= s[1];
        }
        next();
      }
    }

    s[0] = s0;
    s[1] = s1;
  }

  void setSeed(uint64_t seed)
  {
    splitmix64 gen(seed);
    s[0] = gen();
    s[1] = gen();
  }

  void setSeed(uint64_t seed[2])
  {
    s[0] = seed[0];
    s[1] = seed[1];
  }

  template<class T, std::size_t S>
  void shuffle(std::array<T, S> &arr)
  {
    std::uniform_int_distribution<uint64_t> dist(0, static_cast<uint64_t>(arr.size()));

    for (size_t i = static_cast<size_t>(arr.size()) - 1; i >= 1; i--)
    {
      size_t j = dist(*this, std::uniform_int_distribution<uint64_t>::param_type(0, i));
      std::swap(arr[j], arr[i]);
    }
  }

  template<class T>
  void shuffle(std::vector<T> &arr)
  {
    std::uniform_int_distribution<uint64_t> dist(0, static_cast<uint64_t>(arr.size()));

    for (size_t i = static_cast<size_t>(arr.size()) - 1; i >= 1; i--)
    {
      size_t j = dist(*this, std::uniform_int_distribution<uint64_t>::param_type(0, i));
      std::swap(arr[j], arr[i]);
    }
  }
};
