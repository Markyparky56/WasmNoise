#pragma once
#include "defineconfig.hpp"

#include "xoroshiro128plus.hpp"
#include "ReturnArrayHelper.hpp"

class WasmNoise
{
public:
  // I'll get back to this once I have more noise types
  // enum class NoiseType
  // {
  //   Perlin = 0
  // };
  enum class Interp
  {
    Linear = 0,
    Hermite = 1,
    Quintic = 2
  };
  // TODO: FractalType
  // TODO: CellularDistanceFunction
  // TODO: CellularReturnType

  explicit WasmNoise( int32 _seed = 42
                    , WN_DECIMAL _frequency = 0.01
                    , Interp _interp = Interp::Quintic) 
    : frequency(_frequency)
    , interp(_interp)
  { 
    SetSeed(_seed);
  }

  void SetSeed(int32 _seed);
  int32 GetSeed() const { return seed; }

  void SetFrequency(WN_DECIMAL _frequency) { frequency = _frequency; }
  WN_DECIMAL GetFrequency() const { return frequency; }

  void SetInterp(Interp _interp) { interp = _interp; }
  Interp GetInterp() const { return interp; }

  // 2D
  WN_INLINE WN_DECIMAL  GetPerlin(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL *GetPerlinStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length);  
  WN_INLINE WN_DECIMAL *GetPerlinSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);

  // 3D
  WN_INLINE WN_DECIMAL  GetPerlin(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL *GetPerlinStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length);
  WN_INLINE WN_DECIMAL *GetPerlinSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height);
  WN_INLINE WN_DECIMAL *GetPerlinCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);
  
private:
  ReturnArrayHelper returnHelper;

  uint8 perm[512];
  uint8 perm12[512];

  int32 seed;
  WN_DECIMAL frequency;
  Interp interp;

  WN_INLINE WN_DECIMAL SinglePerlin(uint8 offset, WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL SinglePerlin(uint8 offset, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;  
  
  WN_INLINE uint8 Index2D_12(uint8 offset, int32 x, int32 y) const;
  WN_INLINE uint8 Index3D_12(uint8 offset, int32 x, int32 y, int32 z) const;

  WN_INLINE WN_DECIMAL GradCoord2D(uint8 offset, int32 x, int32 y, WN_DECIMAL xd, WN_DECIMAL yd) const;  
  WN_INLINE WN_DECIMAL GradCoord3D(uint8 offset, int32 x, int32 y, int32 z, WN_DECIMAL xd, WN_DECIMAL yd, WN_DECIMAL zd) const;
};

// Declare some log functions as extern so we can call in them in WasmNoise.cpp 
// but import them from javascript
extern "C"
{
  extern void consolelogDecimal(WN_DECIMAL fOut);
  extern void consolelogInt(int32 iOut);
  extern void consolelogStr(char *str);
}
