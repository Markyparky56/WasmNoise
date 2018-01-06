#pragma once
#include "defineconfig.hpp"

#include "xoroshiro128plus.hpp"
#include "ReturnArrayHelper.hpp"

class WasmNoise
{
public:
  // This isn't actually used anywhere yet, until we implement a generic GetNoise function
  enum class NoiseType
  {
    Perlin = 0,
    PerlinFractal = 1,
    Simplex = 2,
    Cellular = 3
  };
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
    RidgedMulti = 2
  };
  enum class CellularDistanceFunction 
  {
    Euclidean = 0,
    Manhattan = 1,
    Natural = 2
  };
  enum class CellularReturnType
  {
    CellValue = 0,
    Distance = 1,
    Distance2 = 2,
    Distance2Add = 3,
    Distance2Sub = 4,
    Distance2Mul = 5,
    Distance2Div = 6,
    NoiseLookupPerlin = 7,    
    NoiseLookupSimplex = 8
    // Other Noise Algorithms 
  };
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
    ZYPlane = 2,
    XWPlane = 3,
    YWPlane = 4,
    ZWPlane = 5,
  };

  explicit WasmNoise( int32 _seed = 42
                    , WN_DECIMAL _frequency = 0.01
                    , Interp _interp = Interp::Quintic
                    , uint32 _fractalOctaves = 3
                    , WN_DECIMAL _fractalLacunarity = WN_DECIMAL(2)
                    , WN_DECIMAL _fractalGain = WN_DECIMAL(0.5)
                    , FractalType _fractalType = FractalType::FBM
                    , CellularDistanceFunction _cellularDistanceFunction = CellularDistanceFunction::Euclidean
                    , CellularReturnType _cellularReturnType = CellularReturnType::CellValue
                    , int32 _cellularDistanceIndex0 = 0
                    , int32 _cellularDistanceIndex1 = 1
                    , WN_DECIMAL _cellularJitter = WN_DECIMAL(0.45)) 
    : frequency(_frequency)
    , interp(_interp)
    , fractalOctaves(_fractalOctaves)
    , fractalLacunarity(_fractalLacunarity)
    , fractalGain(_fractalGain)
    , fractalType(_fractalType)
    , cellularDistanceFunction(_cellularDistanceFunction)
    , cellularReturnType(_cellularReturnType)
    , cellularDistanceIndex0(_cellularDistanceIndex0)
    , cellularDistanceIndex1(_cellularDistanceIndex1)
    , cellularJitter(_cellularJitter)
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

#ifdef WN_INCLUDE_FRACTAL_GETSET
  void SetFractalOctaves(uint32 _octaves) { fractalOctaves = _octaves; CalculateFractalBounding(); }
  uint32 GetFractalOctaves() const { return fractalOctaves; }

  void SetFractalLacunarity(WN_DECIMAL _lacunarity) { fractalLacunarity = _lacunarity; }
  WN_DECIMAL GetFractalLacunarity() const { return fractalLacunarity; }

  void SetFractalGain(WN_DECIMAL _gain) { fractalGain = _gain; CalculateFractalBounding(); }
  WN_DECIMAL GetFractalGain() const { return fractalGain; }

  void SetFractalType(FractalType _fractalType) { fractalType = _fractalType; }
  FractalType GetFractalType() const { return fractalType; }
#endif // WN_INCLUDE_FRACTAL_GETSET

#ifdef WN_INCLUDE_CELLULAR_GETSET
  void SetCellularDistanceFunction(CellularDistanceFunction _cellularDistanceFunction) { cellularDistanceFunction = _cellularDistanceFunction; }
  CellularDistanceFunction GetCellularDistanceFunction() const { return cellularDistanceFunction; }

  void SetCellularReturnType(CellularReturnType _cellularReturnType) { cellularReturnType = _cellularReturnType; }
  CellularReturnType GetCellularReturnType() const { return cellularReturnType; }

  // Sets the 2 distance indices used by distance2 return types
  // Default: 0, 1
  // Note: index0 should be lower than index1
  // Both indices must be >= 0, index1 must be < 4
  // Internally assures this conformance
  void SetCellularDistance2Indices(int32 _cellularDistanceIndex0, int32 _cellularDistanceIndex1);
  void GetCellularDistanceIndex0() const { return cellularDistanceIndex0; }
  void GetCellularDistanceIndex1() const { return cellularDistanceIndex1; }

  void SetCellularJitter(WN_DECIMAL _cellularitter) { cellularJitter = _cellularJitter; }
  WN_DECIMAL GetCellularJitter() const { return cellularJitter; }
#endif


#ifdef WN_INCLUDE_PERLIN
  // 2D
  WN_INLINE WN_DECIMAL  GetPerlin(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL *GetPerlinStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);  
  WN_INLINE WN_DECIMAL *GetPerlinSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);

  // 3D
  WN_INLINE WN_DECIMAL  GetPerlin(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL *GetPerlinStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetPerlinSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetPerlinCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);
#endif // WN_INCLUDE_PERLIN

#ifdef WN_INCLUDE_PERLIN_FRACTAL
  // 2D
  WN_INLINE WN_DECIMAL  GetPerlinFractal(WN_DECIMAL x, WN_DECIMAL y);
  WN_INLINE WN_DECIMAL *GetPerlinFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetPerlinFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);  

  // 3D
  WN_INLINE WN_DECIMAL  GetPerlinFractal(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
  WN_INLINE WN_DECIMAL *GetPerlinFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetPerlinFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetPerlinFractalCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);
#endif // WN_INCLUDE_PERLIN_FRACTAL

#ifdef WN_INCLUDE_SIMPLEX
  // 2D
  WN_INLINE WN_DECIMAL  GetSimplex(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL *GetSimplexStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSimplexSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);

  // 3D
  WN_INLINE WN_DECIMAL  GetSimplex(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL *GetSimplexStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSimplexSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetSimplexCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);

  // 4D
  WN_INLINE WN_DECIMAL  GetSimplex(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w) const;
  WN_INLINE WN_DECIMAL *GetSimplexStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSimplexSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetSimplexCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, uint32 depth);
#endif // WN_INCLUDE_SIMPLEX

#ifdef WN_INCLUDE_SIMPLEX_FRACTAL
  // 2D
  WN_INLINE WN_DECIMAL  GetSimplexFractal(WN_DECIMAL x, WN_DECIMAL y);
  WN_INLINE WN_DECIMAL *GetSimplexFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSimplexFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);

  // 3D
  WN_INLINE WN_DECIMAL  GetSimplexFractal(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
  WN_INLINE WN_DECIMAL *GetSimplexFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSimplexFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetSimplexFractalCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);

  // 4D
  WN_INLINE WN_DECIMAL  GetSimplexFractal(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w);
  WN_INLINE WN_DECIMAL *GetSimplexFractalStrip(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSimplexFractalSquare(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetSimplexFractalCube(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w, uint32 width, uint32 height, uint32 depth);
#endif // WN_INCLUDE_SIMPLEX_FRACTAL

#ifdef WN_INCLUDE_CELLULAR
  // 2D
  WN_INLINE WN_DECIMAL  GetCellular(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL *GetCellularStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetCellularSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);

  // 3D
  WN_INLINE WN_DECIMAL  GetCellular(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL *GetCellularStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetCellularSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetCellularCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);
#endif

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

  CellularDistanceFunction cellularDistanceFunction;
  CellularReturnType cellularReturnType;
  int32 cellularDistanceIndex0;
  int32 cellularDistanceIndex1;
  constexpr int32 CellularDistanceIndexMax = 3;
  WN_DECIMAL cellularJitter;

  void CalculateFractalBounding();

  using Single2DFPtr = WN_DECIMAL(WasmNoise::*)(uint8, WN_DECIMAL, WN_DECIMAL) const; 
  using Single3DFPtr = WN_DECIMAL(WasmNoise::*)(uint8, WN_DECIMAL, WN_DECIMAL, WN_DECIMAL) const;
  using Single4DFPtr = WN_DECIMAL(WasmNoise::*)(uint8, WN_DECIMAL, WN_DECIMAL, WN_DECIMAL, WN_DECIMAL) const;
  using Fractal2DFPtr = WN_DECIMAL(WasmNoise::*)(WN_DECIMAL, WN_DECIMAL);
  using Fractal3DFPtr = WN_DECIMAL(WasmNoise::*)(WN_DECIMAL, WN_DECIMAL, WN_DECIMAL);
  using Fractal4DFPtr = WN_DECIMAL(WasmNoise::*)(WN_DECIMAL, WN_DECIMAL, WN_DECIMAL, WN_DECIMAL);

  // 2D
  WN_INLINE WN_DECIMAL *GetStrip2D(Single2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSquare2D(Single2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);
  WN_INLINE WN_DECIMAL *GetStrip2D(Fractal2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction);  
  WN_INLINE WN_DECIMAL *GetSquare2D(Fractal2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height);

  // 3D
  WN_INLINE WN_DECIMAL *GetStrip3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSquare3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetCube3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);
  WN_INLINE WN_DECIMAL *GetStrip3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSquare3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetCube3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth);

  // 4D
  WN_INLINE WN_DECIMAL *GetStrip4D(Single4DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSquare4D(Single4DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetCube4D(Single4DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, uint32 depth);
  WN_INLINE WN_DECIMAL *GetStrip4D(Fractal4DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 length, StripDirection direction);
  WN_INLINE WN_DECIMAL *GetSquare4D(Fractal4DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, SquarePlane plane);
  WN_INLINE WN_DECIMAL *GetCube4D(Fractal4DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, uint32 depth);

#if defined(WN_INCLUDE_PERLIN_FRACTAL) || defined(WN_INCLUDE_SIMPLEX_FRACTAL)
  // 2D
  WN_INLINE WN_DECIMAL SingleFractalFBM(Single2DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y);
  WN_INLINE WN_DECIMAL SingleFractalBillow(Single2DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y);
  WN_INLINE WN_DECIMAL SingleFractalRidgedMulti(Single2DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y);

  // 3D
  WN_INLINE WN_DECIMAL SingleFractalFBM(Single3DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
  WN_INLINE WN_DECIMAL SingleFractalBillow(Single3DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
  WN_INLINE WN_DECIMAL SingleFractalRidgedMulti(Single3DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);

  // 4D
  WN_INLINE WN_DECIMAL SingleFractalFBM(Single4DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w);
  WN_INLINE WN_DECIMAL SingleFractalBillow(Single4DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w);
  WN_INLINE WN_DECIMAL SingleFractalRidgedMulti(Single4DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w);
#endif

#if defined(WN_INCLUDE_PERLIN) || defined(WN_INCLUDE_PERLIN_FRACTAL)
  // Regular Perlin Noise Functions, necessary for both regular and fractal functions
  WN_INLINE WN_DECIMAL SinglePerlin(uint8 offset, WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL SinglePerlin(uint8 offset, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;  
#endif 

#ifdef WN_INCLUDE_PERLIN_FRACTAL
  // Fractal Perlin Noise Functions
  // 2D
  WN_INLINE WN_DECIMAL SinglePerlinFractalFBM(WN_DECIMAL x, WN_DECIMAL y);
  WN_INLINE WN_DECIMAL SinglePerlinFractalBillow(WN_DECIMAL x, WN_DECIMAL y);
  WN_INLINE WN_DECIMAL SinglePerlinFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y);

  // 3D
  WN_INLINE WN_DECIMAL SinglePerlinFractalFBM(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
  WN_INLINE WN_DECIMAL SinglePerlinFractalBillow(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
  WN_INLINE WN_DECIMAL SinglePerlinFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
#endif // WN_INCLUDE_PERLIN_FRACTAL

#if defined(WN_INCLUDE_SIMPLEX) || defined(WN_INCLUDE_SIMPLEX_FRACTAL)
  // Regular Simplex Noise Functions, necessary for both regular and fractal functions
  WN_INLINE WN_DECIMAL SingleSimplex(uint8 offset, WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL SingleSimplex(uint8 offset, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL SingleSimplex(uint8 offset, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w) const;
#endif

#ifdef WN_INCLUDE_SIMPLEX_FRACTAL
  // Fractal Simplex Noise Functions
  // 2D
  WN_INLINE WN_DECIMAL SingleSimplexFractalFBM(WN_DECIMAL x, WN_DECIMAL y);
  WN_INLINE WN_DECIMAL SingleSimplexFractalBillow(WN_DECIMAL x, WN_DECIMAL y);
  WN_INLINE WN_DECIMAL SingleSimplexFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y);

  // 3D
  WN_INLINE WN_DECIMAL SingleSimplexFractalFBM(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
  WN_INLINE WN_DECIMAL SingleSimplexFractalBillow(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);
  WN_INLINE WN_DECIMAL SingleSimplexFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z);

  // 4D
  WN_INLINE WN_DECIMAL SingleSimplexFractalFBM(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w);
  WN_INLINE WN_DECIMAL SingleSimplexFractalBillow(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w);
  WN_INLINE WN_DECIMAL SingleSimplexFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w);
#endif // WN_INCLUDE_SIMPLEX_FRACTAL

#ifdef WN_INCLUDE_CELLULAR
  // 2D
  WN_INLINE WN_DECIMAL SingleCellular(WN_DECIMAL x, WN_DECIMAL y) const;
  WN_INLINE WN_DECIMAL SingleCellular2Edge(WN_DECIMAL x, WN_DECIMAL y) const;

  // 3D
  WN_INLINE WN_DECIMAL SingleCellular(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
  WN_INLINE WN_DECIMAL SingleCellular2Edge(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const;
#endif

  WN_INLINE uint8 Index2D_12(uint8 offset, int32 x, int32 y) const;
  WN_INLINE uint8 Index3D_12(uint8 offset, int32 x, int32 y, int32 z) const;
  WN_INLINE uint8 Index4D_32(uint8 offset, int32 x, int32 y, int32 z, int32 w) const;
  WN_INLINE uint8 Index2D_256(uint8 offset, int32 x, int32 y) const;
  WN_INLINE uint8 Index3D_256(uint8 offset, int32 x, int32 y, int32 z) const;
  WN_INLINE uint8 Index4D_256(uint8 offset, int32 x, int32 y, int32 z, int32 w) const;

  WN_INLINE WN_DECIMAL GradCoord2D(uint8 offset, int32 x, int32 y, WN_DECIMAL xd, WN_DECIMAL yd) const;  
  WN_INLINE WN_DECIMAL GradCoord3D(uint8 offset, int32 x, int32 y, int32 z, WN_DECIMAL xd, WN_DECIMAL yd, WN_DECIMAL zd) const;
  WN_INLINE WN_DECIMAL GradCoord4D(uint8 offset, int32 x, int32 y, int32 z, int32 w, WN_DECIMAL xd, WN_DECIMAL yd, WN_DECIMAL zd, WN_DECIMAL wd) const;
};

// Declare some log functions as extern so we can call them in WasmNoise.cpp 
// but import them from javascript
extern "C"
{
  extern void consolelogDecimal(WN_DECIMAL fOut);
  extern void consolelogInt(int32 iOut);
  extern void consolelogStr(char *str);
}
