#pragma once
#include "WasmNoise.hpp"
#include "WasmNoise.Common.hpp"
#ifdef WN_INCLUDE_PERLIN_FRACTAL
#include "WasmNoise.Fractal.hpp"
#endif

// Single Noise Function Section *******************************
// These functions are required for both regular and fractal perlin noise

// 2D Perlin
WN_INLINE WN_DECIMAL WasmNoise::SinglePerlin(uint8 offset, WN_DECIMAL x, WN_DECIMAL y) const
{
  int32 x0 = FastFloor(x);
  int32 y0 = FastFloor(y);
  int32 x1 = x0+1;
  int32 y1 = y0+1;

  WN_DECIMAL xs, ys;
  switch(interp)
  {
  case Interp::Linear:
    xs = x - static_cast<WN_DECIMAL>(x0);
    ys = y - static_cast<WN_DECIMAL>(y0);
    break;
  case Interp::Hermite:
    xs = InterpHermiteFunc(x - static_cast<WN_DECIMAL>(x0));
    ys = InterpHermiteFunc(y - static_cast<WN_DECIMAL>(y0));
    break;
  case Interp::Quintic:
    xs = InterpQuinticFunc(x - static_cast<WN_DECIMAL>(x0));
    ys = InterpQuinticFunc(y - static_cast<WN_DECIMAL>(y0));
    break;
  }  
  
  WN_DECIMAL xd0 = x - static_cast<WN_DECIMAL>(x0);
  WN_DECIMAL yd0 = y - static_cast<WN_DECIMAL>(y0);
  WN_DECIMAL xd1 = xd0 - 1;
  WN_DECIMAL yd1 = yd0 - 1;

  WN_DECIMAL xf0 = Lerp(GradCoord2D(offset, x0, y0, xd0, yd0), GradCoord2D(offset, x1, y0, xd1, yd0), xs);
  WN_DECIMAL xf1 = Lerp(GradCoord2D(offset, x0, y1, xd0, yd1), GradCoord2D(offset, x1, y1, xd1, yd1), xs);

  return Lerp(xf0, xf1, ys);
}

// 3D Perlin
WN_INLINE WN_DECIMAL WasmNoise::SinglePerlin(uint8 offset, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const
{
  int32 x0 = FastFloor(x);
  int32 y0 = FastFloor(y);
  int32 z0 = FastFloor(z);
  int32 x1 = x0 + 1;
  int32 y1 = y0 + 1;
  int32 z1 = z0 + 1;

  WN_DECIMAL xs, ys, zs;
  switch(interp)
  {
  case Interp::Linear:
    xs = x - static_cast<WN_DECIMAL>(x0);
    ys = y - static_cast<WN_DECIMAL>(y0);
    zs = z - static_cast<WN_DECIMAL>(z0);
    break;
  case Interp::Hermite:
    xs = InterpHermiteFunc(x - static_cast<WN_DECIMAL>(x0));
    ys = InterpHermiteFunc(y - static_cast<WN_DECIMAL>(y0));
    zs = InterpHermiteFunc(z - static_cast<WN_DECIMAL>(z0));
    break;
  case Interp::Quintic:
    xs = InterpQuinticFunc(x - static_cast<WN_DECIMAL>(x0));
    ys = InterpQuinticFunc(y - static_cast<WN_DECIMAL>(y0));
    zs = InterpQuinticFunc(z - static_cast<WN_DECIMAL>(z0));  
    break;
  }
  
  WN_DECIMAL xd0 = x - static_cast<WN_DECIMAL>(x0);
  WN_DECIMAL yd0 = y - static_cast<WN_DECIMAL>(y0);
  WN_DECIMAL zd0 = z - static_cast<WN_DECIMAL>(z0);
  WN_DECIMAL xd1 = xd0 - 1;
  WN_DECIMAL yd1 = yd0 - 1;
  WN_DECIMAL zd1 = zd0 - 1;

  WN_DECIMAL xf00 = Lerp(GradCoord3D(offset, x0, y0, z0, xd0, yd0, zd0), GradCoord3D(offset, x1, y0, z0, xd1, yd0, zd0), xs);
  WN_DECIMAL xf10 = Lerp(GradCoord3D(offset, x0, y1, z0, xd0, yd1, zd0), GradCoord3D(offset, x1, y1, z0, xd1, yd1, zd0), xs);
  WN_DECIMAL xf01 = Lerp(GradCoord3D(offset, x0, y0, z1, xd0, yd0, zd1), GradCoord3D(offset, x1, y0, z1, xd1, yd0, zd1), xs);
  WN_DECIMAL xf11 = Lerp(GradCoord3D(offset, x0, y1, z1, xd0, yd1, zd1), GradCoord3D(offset, x1, y1, z1, xd1, yd1, zd1), xs);

  WN_DECIMAL yf0 = Lerp(xf00, xf10, ys);
  WN_DECIMAL yf1 = Lerp(xf01, xf11, ys);

  return Lerp(yf0, yf1, zs);
}

#ifdef WN_INCLUDE_PERLIN_FRACTAL
// 2D Perlin Fractal Functions
WN_INLINE WN_DECIMAL WasmNoise::SinglePerlinFractalFBM(WN_DECIMAL x, WN_DECIMAL y) 
{
  return SingleFractalFBM<Single2DFPtr>(&WasmNoise::SinglePerlin, x, y);
}

WN_INLINE WN_DECIMAL WasmNoise::SinglePerlinFractalBillow(WN_DECIMAL x, WN_DECIMAL y) 
{
  return SingleFractalBillow<Single2DFPtr>(&WasmNoise::SinglePerlin, x, y);
}

WN_INLINE WN_DECIMAL WasmNoise::SinglePerlinFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y) 
{
  return SingleFractalRidgedMulti<Single2DFPtr>(&WasmNoise::SinglePerlin, x, y);
}

// 3D Perlin Fractal Functions
WN_INLINE WN_DECIMAL WasmNoise::SinglePerlinFractalFBM(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  return SingleFractalFBM<Single3DFPtr>(&WasmNoise::SinglePerlin, x, y, z);
}

WN_INLINE WN_DECIMAL WasmNoise::SinglePerlinFractalBillow(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  return SingleFractalBillow<Single3DFPtr>(&WasmNoise::SinglePerlin, x, y, z);  
}

WN_INLINE WN_DECIMAL WasmNoise::SinglePerlinFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  return SingleFractalRidgedMulti<Single3DFPtr>(&WasmNoise::SinglePerlin, x, y, z);
}
#endif // WN_INCLUDE_PERLIN_FRACTAL

// End Single Noise Function Section ***************************

// Public Perlin Noise functions

#ifdef WN_INCLUDE_PERLIN
// 2D Single
WN_INLINE WN_DECIMAL WasmNoise::GetPerlin(WN_DECIMAL x, WN_DECIMAL y) const
{
  return SinglePerlin(0, x * frequency, y * frequency);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction)
{
  return GetStrip2D(&WasmNoise::SinglePerlin, startX, startY, length, direction);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height)  
{  
  return GetSquare2D(&WasmNoise::SinglePerlin, startX, startY, width, height);
}

// 3D Single
WN_INLINE WN_DECIMAL WasmNoise::GetPerlin(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const
{
  return SinglePerlin(0, x * frequency, y * frequency, z * frequency);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction)
{
  return GetStrip3D(&WasmNoise::SinglePerlin, startX, startY, startZ, length, direction);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane)
{
  return GetSquare3D(&WasmNoise::SinglePerlin, startX, startY, startZ, width, height, plane);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth)
{
  return GetCube3D(&WasmNoise::SinglePerlin, startX, startY, startZ, width, height, depth);
}
#endif // WN_INCLUDE_PERLIN

#ifdef WN_INCLUDE_PERLIN_FRACTAL
// 2D Fractal
WN_INLINE WN_DECIMAL WasmNoise::GetPerlinFractal(WN_DECIMAL x, WN_DECIMAL y)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return SinglePerlinFractalFBM(x * frequency, y * frequency);
  case FractalType::Billow:       return SinglePerlinFractalBillow(x * frequency, y * frequency);
  case FractalType::RidgedMulti:  return SinglePerlinFractalRidgedMulti(x * frequency, y * frequency);
  default:
    ABORT();
    return 0;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction)
{  
  switch(fractalType)
  {
  case FractalType::FBM:          return GetStrip2D(&WasmNoise::SinglePerlinFractalFBM, startX, startY, length, direction);
  case FractalType::Billow:       return GetStrip2D(&WasmNoise::SinglePerlinFractalBillow, startX, startY, length, direction);
  case FractalType::RidgedMulti:  return GetStrip2D(&WasmNoise::SinglePerlinFractalRidgedMulti, startX, startY, length, direction);
  default:
      ABORT();
      return nullptr;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetSquare2D(&WasmNoise::SinglePerlinFractalFBM, startX, startY, width, height);
  case FractalType::Billow:       return GetSquare2D(&WasmNoise::SinglePerlinFractalBillow, startX, startY, width, height);
  case FractalType::RidgedMulti:  return GetSquare2D(&WasmNoise::SinglePerlinFractalRidgedMulti, startX, startY, width, height);
  default:
    ABORT();
    return nullptr;
  }
}

// 3D Fractal
WN_INLINE WN_DECIMAL WasmNoise::GetPerlinFractal(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return SinglePerlinFractalFBM(x *frequency, y * frequency, z * frequency);
  case FractalType::Billow:       return SinglePerlinFractalBillow(x * frequency, y * frequency, z * frequency);
  case FractalType::RidgedMulti:  return SinglePerlinFractalRidgedMulti(x * frequency, y * frequency, z * frequency);
  default:
    ABORT();
    return 0;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetStrip3D(&WasmNoise::SinglePerlinFractalFBM, startX, startY, startZ, length, direction);
  case FractalType::Billow:       return GetStrip3D(&WasmNoise::SinglePerlinFractalBillow, startX, startY, startZ, length, direction);
  case FractalType::RidgedMulti:  return GetStrip3D(&WasmNoise::SinglePerlinFractalRidgedMulti, startX, startY, startZ, length, direction);
  default:
    ABORT();
    return nullptr;
  }  
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetSquare3D(&WasmNoise::SinglePerlinFractalFBM, startX, startY, startZ, width, height, plane);
  case FractalType::Billow:       return GetSquare3D(&WasmNoise::SinglePerlinFractalBillow, startX, startY, startZ, width, height, plane);
  case FractalType::RidgedMulti:  return GetSquare3D(&WasmNoise::SinglePerlinFractalRidgedMulti, startX, startY, startZ, width, height, plane);
  default:
    ABORT();
    return nullptr;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetPerlinFractalCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetCube3D(&WasmNoise::SinglePerlinFractalFBM, startX, startY, startZ, width, height, depth);
  case FractalType::Billow:       return GetCube3D(&WasmNoise::SinglePerlinFractalBillow, startX, startY, startZ, width, height, depth);
  case FractalType::RidgedMulti:  return GetCube3D(&WasmNoise::SinglePerlinFractalRidgedMulti, startX, startY, startZ, width, height, depth);
  default:
    ABORT();
    return nullptr;
  }
}
#endif // WN_INCLUDE_PERLIN_FRACTAL
