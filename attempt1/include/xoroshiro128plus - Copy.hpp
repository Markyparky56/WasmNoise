#pragma once
// xoroshiro128plus class, converted from xoroshift128plus.c 
// by David Blackman & Sebastino Vigna
// Original file can be found here: http://xoroshiro.di.unimi.it/xoroshiro128plus.c
// Conversion by Mark A. Ropper
// Attempted to convert to 32bit for Emscripten, since Embind does not support long long (uint64_t)

#include <cstdint>
#include <random>
#include "splitmix64.hpp"
#include <array>
#include <vector>
#include <utility> // std::swap
#include <random>

using std::uint32_t;

class xoroshiro128plus
{
private:
  uint32_t s[2];

  static inline uint32_t rotl(const uint32_t x, int k)
  {
    return (x << k) | (x >> (32 - k));
  }

  inline uint32_t next()
  {
    const uint32_t s0 = s[0];
    uint32_t s1 = s[1];
    const uint32_t result = s0 + s1;

    s1 ^= s0;
    s[0] = rotl(s0, 27) ^ s1 ^ (s1 << 7); // a, b
    s[1] = rotl(s1, 18);

    return result;
  }

public:
  typedef uint32_t result_type; // Attempt to interface with the STL's uniform distributions
  static constexpr uint32_t min() { return std::numeric_limits<uint32_t>::min(); }
  static constexpr uint32_t max() { return std::numeric_limits<uint32_t>::max(); }

  xoroshiro128plus()
    : s{ std::random_device()(), std::random_device()() }
  {}
  xoroshiro128plus(uint32_t state[2])
    : s{ state[0], state[1]}
  {}
  // Recommended setup from xoroshiro128plus.c
  xoroshiro128plus(uint32_t seed)
  {
    setSeed(seed);
  }

  inline uint32_t operator()()
  {
    return next();
  }

  inline uint32_t get()
  {
    return next();
  }

  // This is the jump function for the generator. It is equivalent to
  // 2^64 calls to next(), it can be used to generate 2^64 non-overlapping
  // subsequences for parallel computations
  void jump()
  {
    static const uint32_t JUMP[] = { 0xbeac0467, 0xd86b048b };

    uint32_t s0 = 0;
    uint32_t s1 = 0;
    for (int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
    {
      for (int b = 0; b < 64; b++)
      {
        if (JUMP[i] & UINT32_C(1) << b)
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
    splitmix32 gen(seed);
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
    std::uniform_int_distribution<uint32_t> dist(0, static_cast<uint32_t>(arr.size()));

    for (size_t i = static_cast<size_t>(arr.size()) - 1; i >= 1; i--)
    {
      size_t j = dist(*this, std::uniform_int_distribution<uint32_t>::param_type(0, i));
      std::swap(arr[j], arr[i]);
    }
  }

  template<class T>
  void shuffle(std::vector<T> &arr)
  {
    std::uniform_int_distribution<uint32_t> dist(0, static_cast<uint32_t>(arr.size()));

    for (size_t i = static_cast<size_t>(arr.size()) - 1; i >= 1; i--)
    {
      size_t j = dist(*this, std::uniform_int_distribution<uint32_t>::param_type(0, i));
      std::swap(arr[j], arr[i]);
    }
  }

  template<typename T, std::size_t S>
  void shuffle(T arr[S])
  {
    std::uniform_int_distribution<uint32_t> dist(0, S);

    for(size_t i = S-1; i >= 1; i--)
    {
      size_t j = dist(*this, std::uniform_int_distribution<uint32_t>::param_type(0, i));
      std::swap(arr[j], arr[i]);
    }
  }
};
