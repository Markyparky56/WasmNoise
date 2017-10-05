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

#include "xoroshiro128plus.hpp"

class WasmNoise
{
public:
  explicit WasmNoise(int32 _seed = 42, WM_DECIMAL _frequency = 0.01) 
    : frequency(_frequency)
  { 
    SetSeed(_seed);
  }

  // WebAssembly doesn't seem to call the static object's constructor so everything
  // is left 0-initialised, so move I'm moving all the default setting to this function
  // and exporting a call to this function through the interface so it can be called
  // first when the module is loaded
  void Initialise()
  {
    frequency = 0.01;
    SetSeed(42);
  }

  void SetSeed(int32 _seed);
  int32 GetSeed() const { return seed; }

  void SetFrequency(WM_DECIMAL _frequency) { frequency = _frequency; }
  WM_DECIMAL GetFrequency() const { return frequency; }

  WM_DECIMAL GetPerlin(WM_DECIMAL x, WM_DECIMAL y) const;  
  WM_DECIMAL GetPerlin_log(WM_DECIMAL x, WM_DECIMAL y) const;  
  
private:
  uint8 perm[512];
  uint8 perm12[512];

  int32 seed = 42;
  WM_DECIMAL frequency;

  WM_DECIMAL SinglePerlin(uint8 offset, WM_DECIMAL x, WM_DECIMAL y) const;  
  WM_DECIMAL SinglePerlin_log(uint8 offset, WM_DECIMAL x, WM_DECIMAL y) const;  
  
  WM_INLINE uint8 Index2D_12(uint8 offset, int32 x, int32 y) const;

  WM_INLINE WM_DECIMAL GradCoord2D(uint8 offset, int32 x, int32 y, WM_DECIMAL xd, WM_DECIMAL yd) const;  
};

// Declare some log functions as extern so we can call in them in WasmNoise.cpp 
// but import them from javascript
extern "C"
{
  extern void consolelogDecimal(WM_DECIMAL fOut);
  extern void consolelogInt(int32 iOut);
  extern void consolelogStr(char *str);
}
