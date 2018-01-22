#pragma once
#include "defineconfig.hpp"
#include "types.hpp"

constexpr WN_DECIMAL GRAD_X[] =
{
  1, -1, 1, -1,
  1, -1, 1, -1,
  0,  0, 0,  0
};

constexpr WN_DECIMAL GRAD_Y[] =
{
  1,  1, -1, -1,
  0,  0,  0,  0,
  1, -1,  1, -1
};

constexpr WN_DECIMAL GRAD_Z[] =
{
  0, 0,  0,  0,
  1, 1, -1, -1,
  1, 1, -1, -1
};

constexpr WN_DECIMAL GRAD_4D[] =
{
  0,1,1,1,0,1,1,-1,0,1,-1,1,0,1,-1,-1,
  0,-1,1,1,0,-1,1,-1,0,-1,-1,1,0,-1,-1,-1,
  1,0,1,1,1,0,1,-1,1,0,-1,1,1,0,-1,-1,
  -1,0,1,1,-1,0,1,-1,-1,0,-1,1,-1,0,-1,-1,
  1,1,0,1,1,1,0,-1,1,-1,0,1,1,-1,0,-1,
  -1,1,0,1,-1,1,0,-1,-1,-1,0,1,-1,-1,0,-1,
  1,1,1,0,1,1,-1,0,1,-1,1,0,1,-1,-1,0,
  -1,1,1,0,-1,1,-1,0,-1,-1,1,0,-1,-1,-1,0
};

constexpr int32 XPrime = 1619;
constexpr int32 YPrime = 31337;
constexpr int32 ZPrime = 6971;
constexpr int32 WPrime = 1013;

static WN_INLINE int32 FastFloor(WN_DECIMAL f) { return (f >= 0 ? static_cast<int32>(f) : static_cast<int32>(f) - 1); }
static WN_INLINE int32 FastRound(WN_DECIMAL f) { return (f >= 0 ? static_cast<int32>(f + WN_DECIMAL(0.5)) : static_cast<int32>(f - WN_DECIMAL(0.5))); }
static WN_INLINE int32 FastAbs(int32 i) { return __builtin_labs(i); }
static WN_INLINE WN_DECIMAL FastAbs(WN_DECIMAL f){ return __builtin_fabs(f); }
static WN_INLINE WN_DECIMAL Lerp(WN_DECIMAL a, WN_DECIMAL b, WN_DECIMAL t) { return a + t * (b - a); }
static WN_INLINE WN_DECIMAL InterpHermiteFunc(WN_DECIMAL t) { return t*t*(3 - 2*t); }
static WN_INLINE WN_DECIMAL InterpQuinticFunc(WN_DECIMAL t) { return t*t*t*(t*(t*6 -15) + 10); }
static WN_INLINE WN_DECIMAL CubicLerp(WN_DECIMAL a, WN_DECIMAL b, WN_DECIMAL c, WN_DECIMAL d, WN_DECIMAL t)
{
  WN_DECIMAL p = (d-c)-(a-b);
  return t*t*t*p + t*t*((a-b)-p) + t*(c-a) + b;
}

template<class T> static WN_INLINE T min(T a, T b) { return (a < b) ? a : b; }
template<class T> static WN_INLINE T max(T a, T b) { return (a > b) ? a : b; }

static WN_INLINE WN_DECIMAL ValCoord2D(int32 seed, int32 x, int32 y)
{
  int32 n = seed;
  n ^= XPrime * x;
  n ^= YPrime * y;

  return (n * n * n * 60493) / WN_DECIMAL(2147483648);
}

static WN_INLINE WN_DECIMAL ValCoord3D(int32 seed, int32 x, int32 y, int32 z)
{
  int32 n = seed;
  n ^= XPrime * x;
  n ^= YPrime * y;
  n ^= ZPrime * z;

  return (n * n * n * 60493) / WN_DECIMAL(2147483648);
}

static WN_INLINE WN_DECIMAL ValCoord4D(int32 seed, int32 x, int32 y, int32 z, int32 w)
{
  int32 n = seed;
  n ^= XPrime * x;
  n ^= YPrime * y;
  n ^= ZPrime * z;
  n ^= WPrime * w;

  return (n * n * n * 60493) / WN_DECIMAL(2147483648);
}
