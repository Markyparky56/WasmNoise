#pragma once
// xoroshiro128plus class, converted from xoroshift128plus.c 
// by David Blackman & Sebastino Vigna
// Original file can be found here: http://xoroshiro.di.unimi.it/xoroshiro128plus.c
// Conversion by Mark A. Ropper

#include "splitmix64.hpp"
#include "types.hpp"
#include "numeric_limits.hpp"

class xoroshiro128plus
{
private:
  uint64 s[2];

  static inline uint64 rotl(const uint64 x, int k)
  {
    return (x << k) | (x >> (64 - k));
  }

  inline uint64 next()
  {
    const uint64 s0 = s[0];
    uint64 s1 = s[1];
    const uint64 result = s0 + s1;

    s1 ^= s0;
    s[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
    s[1] = rotl(s1, 36);

    return result;
  }

public:
  typedef uint64 result_type; // Attempt to interface with the STL's uniform distributions
  static constexpr uint64 min() { return numeric_limits<uint64>::min(); }
  static constexpr uint64 max() { return numeric_limits<uint64>::max(); }

  // xoroshiro128plus()
  //   : s{ std::random_device()(), std::random_device()() }
  // {}
  xoroshiro128plus(uint64 state[2])
    : s{ state[0], state[1]}
  {}
  // Recommended setup from xoroshiro128plus.c
  xoroshiro128plus(uint64 seed)
  {
    setSeed(seed);
  }

  inline uint64 operator()()
  {
    return next();
  }

  inline uint64 get()
  {
    return next();
  }

  // This is the jump function for the generator. It is equivalent to
  // 2^64 calls to next(), it can be used to generate 2^64 non-overlapping
  // subsequences for parallel computations
  void jump()
  {
    static const uint64 JUMP[] = { 0xbeac0467eba5facb, 0xd86b048b86aa9922 };

    uint64 s0 = 0;
    uint64 s1 = 0;
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

  void setSeed(uint64 seed)
  {
    splitmix64 gen(seed);
    s[0] = gen();
    s[1] = gen();
  }

  void setSeed(uint64 seed[2])
  {
    s[0] = seed[0];
    s[1] = seed[1];
  }

  // template<class T, std::size_t S>
  // void shuffle(std::array<T, S> &arr)
  // {
  //   std::uniform_int_distribution<uint64> dist(0, static_cast<uint64>(arr.size()));

  //   for (uint64 i = static_cast<size_t>(arr.size()) - 1; i >= 1; i--)
  //   {
  //     size_t j = dist(*this, std::uniform_int_distribution<uint64>::param_type(0, i));
  //     std::swap(arr[j], arr[i]);
  //   }
  // }

  // template<class T>
  // void shuffle(std::vector<T> &arr)
  // {
  //   std::uniform_int_distribution<uint64_t> dist(0, static_cast<uint64_t>(arr.size()));

  //   for (size_t i = static_cast<size_t>(arr.size()) - 1; i >= 1; i--)
  //   {
  //     size_t j = dist(*this, std::uniform_int_distribution<uint64_t>::param_type(0, i));
  //     std::swap(arr[j], arr[i]);
  //   }
  // }
};
