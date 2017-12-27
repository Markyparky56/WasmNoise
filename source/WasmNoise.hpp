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
  enum class FractalType 
  { 
    FBM = 0, 
    Billow = 1, 
    RigidMulti = 2
  };
  // TODO: CellularDistanceFunction
  // TODO: CellularReturnType
  enum class StripDirection
  {
    XAxis = 0,
    YAxis = 1,
    ZAxis = 2,
    WAxis = 3
  };
  enum class SquarePlane
  {
    XYPlane = 0,
    XZPlane = 1,
    ZYPlane = 2
  };

  explicit WasmNoise( int32 _seed = 42
                    , WN_DECIMAL _frequency = 0.01
                    , Interp _interp = Interp::Quintic
                    , uint32 _fractalOctaves = 3
                    , WN_DECIMAL _fractalLacunarity = WN_DECIMAL(2)
                    , WN_DECIMAL _fractalGain = WN_DECIMAL(0.5)
                    , FractalType _fractalType = FractalType::FBM) 
    : frequency(_frequency)
    , interp(_interp)
    , fractalOctaves(_fractalOctaves)
    , fractalLacunarity(_fractalLacunarity)
    , fractalGain(_fractalGain)
    , fractalType(_fractalType)
  { 
    SetSeed(_seed);
    CalculateFractalBounding();
  }

  void SetSeed(int32 _seed);
  int32 GetSeed() const { return seed; }

  void SetFrequency(WN_DECIMAL _frequency) { frequency = _frequency; }
  WN_DECIMAL GetFrequency() const { return frequency; }

  void SetInterp(Interp _interp) { interp = _interp; }
  Interp GetInterp() const { return interp; }

  void SetFractalOctaves(uint32 _octaves) { fractalOctaves = _octaves; CalculateFractalBounding(); }
  uint32 GetFractalOctaves() { return fractalOctaves; }

  void SetFractalLacunarity(WN_DECIMAL _lacunarity) { fractalLacunarity = _lacunarity; }
  WN_DECIMAL GetFractalLacunarity() { return fractalLacunarity; }

  void SetFractalGain(WN_DECIMAL _gain) { fractalGain = _gain; CalculateFractalBounding(); }
  WN_DECIMAL GetFractalGain() { return fractalGain; }

  void SetFractalType(FractalType _fractalType) { fractalType = _fractalType; }
  FractalType GetFractalType() { return fractalType; }

  // 2D
  WN_INLINE WN_DECIMAL  GetPerlin(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL *GetPerlinStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);  
  WN_INLINE WN_DECIMAL *GetPerlinSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);

  WN_INLINE WN_DECIMAL  GetPerlinFractal(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL *GetPerlinFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetPerlinFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);  

  // 3D
  WN_INLINE WN_DECIMAL  GetPerlin(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL *GetPerlinStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetPerlinSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetPerlinCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);

  WN_INLINE WN_DECIMAL  GetPerlinFractal(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL *GetPerlinFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetPerlinFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetPerlinFractalCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);
  
private:
  ReturnArrayHelper returnHelper;

  uint8 perm[512];
  uint8 perm12[512];

  int32 seed;
  WN_DECIMAL frequency;
  Interp interp;
  uint32 fractalOctaves;
  WN_DECIMAL fractalLacunarity;
  WN_DECIMAL fractalGain;
  FractalType fractalType;
  WN_DECIMAL fractalBounding;

  void CalculateFractalBounding();

  using Single2DFPtr = WN_DECIMAL(WasmNoise::*)(uint8, WN_DECIMAL, WN_DECIMAL) const; 
  using Single3DFPtr = WN_DECIMAL(WasmNoise::*)(uint8, WN_DECIMAL, WN_DECIMAL, WN_DECIMAL) const;
  using Fractal2DFPtr = WN_DECIMAL(WasmNoise::*)(WN_DECIMAL, WN_DECIMAL) const;
  using Fractal3DFPtr = WN_DECIMAL(WasmNoise::*)(WN_DECIMAL, WN_DECIMAL, WN_DECIMAL) const;

  WN_INLINE WN_DECIMAL *GetStrip2D(Single2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSquare2D(Single2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);
  WN_INLINE WN_DECIMAL *GetStrip2D(Fractal2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);  
  WN_INLINE WN_DECIMAL *GetSquare2D(Fractal2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);

  WN_INLINE WN_DECIMAL *GetStrip3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSquare3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetCube3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);
  WN_INLINE WN_DECIMAL *GetStrip3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSquare3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetCube3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);

  // Regular Noise Functions
  WN_INLINE WN_DECIMAL SinglePerlin(uint8 offset, WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL SinglePerlin(uint8 offset, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;  

  // Fractal Noise Functions
  WN_INLINE WN_DECIMAL SinglePerlinFractalFBM(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL SinglePerlinFractalBillow(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL SinglePerlinFractalRigidMulti(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL SinglePerlinFractalFBM(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL SinglePerlinFractalBillow(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL SinglePerlinFractalRigidMulti(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  
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
