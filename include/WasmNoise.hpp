#pragma once

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#define WM_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define WM_INLINE __forceinline 
#else
#define WM_INLINE inline
#endif

#ifdef WM_USE_DOUBLES
using WM_DECIMAL = double;
#else
using WM_DECIMAL = float;
#endif

using uint8 = unsigned char;
using int8 = char;
using uint32 = unsigned int;
using int32 = int;

#include "xoroshiro128plus.hpp"

class WasmNoise
{
public:
  explicit WasmNoise(int32 seed = 42) { SetSeed(seed); }

  void SetSeed(int32 _seed);
  int32 GetSeed() const { return seed ;}

  WM_DECIMAL GetPerlin(WM_DECIMAL x, WM_DECIMAL y) const;

private:
  uint8 perm[512];
  uint8 perm12[512];

  int32 seed = 42;

  WM_DECIMAL SinglePerlin(uint8 offset, WM_DECIMAL x, WM_DECIMAL y) const;  

  WM_INLINE uint8 Index2D_12(uint8 offset, int32 x, int32 y) const;

  WM_INLINE WM_DECIMAL GradCoord2D(uint8 offset, int32 x, int32 y, WM_DECIMAL xd, WM_DECIMAL yd) const;  
};
