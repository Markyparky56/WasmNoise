#pragma once
#include "WasmNoise.hpp"
#include "WasmNoise.Common.hpp"
#ifdef WN_INCLUDE_SIMPLEX_FRACTAL
#include "WasmNoise.Fractal.hpp"
#endif

// Single Noise Function Section *******************************
// These functions are required for both regular and fractal simplex noise

static constexpr WN_DECIMAL SQRT3 = WN_DECIMAL(1.7320508075688772935274463415059);
static constexpr WN_DECIMAL SQRT5 = WN_DECIMAL(2.236067977499789696409173668731);
static constexpr WN_DECIMAL F2 = WN_DECIMAL(0.5) * (SQRT3 - WN_DECIMAL(1.0));
static constexpr WN_DECIMAL G2 = (WN_DECIMAL(3.0) - SQRT3) / WN_DECIMAL(6.0);
static constexpr WN_DECIMAL F3 = 1 / WN_DECIMAL(3);
static constexpr WN_DECIMAL G3 = 1 / WN_DECIMAL(6);
static constexpr WN_DECIMAL F4 = (SQRT5 - 1) / 4;
static constexpr WN_DECIMAL G4 = (5 - SQRT5) / 20;

static const uint8 SIMPLEX_4D[] =
{
  0,1,2,3,0,1,3,2,0,0,0,0,0,2,3,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,0,
	0,2,1,3,0,0,0,0,0,3,1,2,0,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,3,2,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,2,0,3,0,0,0,0,1,3,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,3,0,1,2,3,1,0,
	1,0,2,3,1,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,3,1,0,0,0,0,2,1,3,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	2,0,1,3,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,2,3,0,2,1,0,0,0,0,3,1,2,0,
	2,1,0,3,0,0,0,0,0,0,0,0,0,0,0,0,3,1,0,2,0,0,0,0,3,2,0,1,3,2,1,0
};

// 2D
WN_INLINE WN_DECIMAL WasmNoise::SingleSimplex(WN_DECIMAL x, WN_DECIMAL y, uint8 offset) const
{
  WN_DECIMAL t = (x + y) * F2;
  int32 i = FastFloor(x + t);
  int32 j = FastFloor(y + t);

  t = (i + j) * G2;
  WN_DECIMAL X0 = i - t;
  WN_DECIMAL Y0 = j - t;

  WN_DECIMAL x0 = x - X0;
  WN_DECIMAL y0 = y - Y0;

  int32 i1, j1;
  if(x0 > y0)
  {
    i1 = 1, j1 = 0;
  }
  else
  {
    i1 = 0; j1 = 1;
  }

  WN_DECIMAL x1 = x0 - static_cast<WN_DECIMAL>(i1) + G2;
  WN_DECIMAL y1 = y0 - static_cast<WN_DECIMAL>(j1) + G2;
  WN_DECIMAL x2 = x0 - 1 + 2*G2;
  WN_DECIMAL y2 = y0 - 1 + 2*G2;

  WN_DECIMAL n0, n1, n2;

  t = static_cast<WN_DECIMAL>(0.5) - x0*x0 - y0*y0;
  if(t < 0) n0 = 0;
  else
  {
    t *= t;
    n0 = t * t * GradCoord2D(offset, i, j, x0, y0);
  }

  t = static_cast<WN_DECIMAL>(0.5) - x1*x1 - y1*y1;
  if(t < 0) n1 = 0;
  else
  {
    t *= t;
    n1 = t * t * GradCoord2D(offset, i + i1, j + j1, x1, y1);
  }

  t = static_cast<WN_DECIMAL>(0.5) - x2*x2 - y2*y2;
  if(t < 0) n2 = 0;
  else
  {
    t *= t;
    n2 = t * t * GradCoord2D(offset, i + 1, j + 1, x2, y2);
  }

  return 70 * (n0 + n1 + n2);
}

// 3D
WN_INLINE WN_DECIMAL WasmNoise::SingleSimplex(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, uint8 offset) const
{
  WN_DECIMAL t = (x + y + z) * F3;
  int32 i = FastFloor(x + t);
  int32 j = FastFloor(y + t);
  int32 k = FastFloor(z + t);

  t = (i + j + k) * G3;
  WN_DECIMAL X0 = i - t;
  WN_DECIMAL Y0 = j - t;
  WN_DECIMAL Z0 = k - t;

  WN_DECIMAL x0 = x - X0;
  WN_DECIMAL y0 = y - Y0;
  WN_DECIMAL z0 = z - Z0;

  int32 i1, j1, k1;
  int32 i2, j2, k2;

  if (x0 >= y0)
  {
    if (y0 >= z0)
    {
      i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
    }
    else if (x0 >= z0)
    {
      i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;
    }
    else // x0 < z0
    {
      i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;
    }
  }
  else // x0 < y0
  {
    if (y0 < z0)
    {
      i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;
    }
    else if (x0 < z0)
    {
      i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
    }
    else // x0 >= z0
    {
      i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
    }
  }

  WN_DECIMAL x1 = x0 - i1 + G3;
  WN_DECIMAL y1 = y0 - j1 + G3;
  WN_DECIMAL z1 = z0 - k1 + G3;
  WN_DECIMAL x2 = x0 - i2 + 2*G3;
  WN_DECIMAL y2 = y0 - j2 + 2*G3;
  WN_DECIMAL z2 = z0 - k2 + 2*G3;
  WN_DECIMAL x3 = x0 - 1 + 3*G3;
  WN_DECIMAL y3 = y0 - 1 + 3*G3;
  WN_DECIMAL z3 = z0 - 1 + 3*G3;

  WN_DECIMAL n0, n1, n2, n3;

  t = static_cast<WN_DECIMAL>(0.6) - x0*x0 - y0*y0 - z0*z0;
  if (t < 0) n0 = 0;
  else
  {
    t *= t;
    n0 = t * t * GradCoord3D(offset, i, j, k, x0, y0, z0);
  }

  t = static_cast<WN_DECIMAL>(0.6) - x1*x1 - y1*y1 - z1*z1;
  if (t < 0) n1 = 0;
  else
  {
    t *= t;
    n1 = t * t * GradCoord3D(offset, i + i1, j + j1, k + k1, x1, y1, z1);
  }

  t = static_cast<WN_DECIMAL>(0.6) - x2*x2 - y2*y2 - z2*z2;
  if (t < 0) n2 = 0;
  else
  {
    t *= t;
    n2 = t * t * GradCoord3D(offset, i + i2, j + j2, k + k2, x2, y2, z2);
  }

  t = static_cast<WN_DECIMAL>(0.6) - x3*x3 - y3*y3 - z3*z3;
  if (t < 0) n3 = 0;
  else
  {
    t *= t;
    n3 = t * t * GradCoord3D(offset, i + 1, j + 1, k + 1, x3, y3, z3);
  }

  return 32 * (n0 + n1 + n2 + n3);
}

// 4D
WN_INLINE WN_DECIMAL WasmNoise::SingleSimplex(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w, uint8 offset) const
{
  WN_DECIMAL n0, n1, n2, n3, n4;
  WN_DECIMAL t = (x + y + z + w) * F4;
  int i = FastFloor(x + t);
  int j = FastFloor(y + t);
  int k = FastFloor(z + t);
  int l = FastFloor(w + t);  
  t = (i + j + k + l) * G4;
  WN_DECIMAL X0 = i - t;
  WN_DECIMAL Y0 = j - t;
  WN_DECIMAL Z0 = k - t;
  WN_DECIMAL W0 = l -t;
  WN_DECIMAL x0 = x - X0;
  WN_DECIMAL y0 = y - Y0;
  WN_DECIMAL z0 = z - Z0;
  WN_DECIMAL w0 = w - W0;

  int c = (x0 > y0) ? 32 : 0;
  c += (x0 > z0) ? 16 : 0;
  c += (y0 > z0) ? 8 : 0;
  c += (x0 > w0) ? 4 : 0;
  c += (y0 > w0) ? 2 : 0;
  c += (z0 > w0) ? 1 : 0;
  c <<= 2;

  int i1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
  int i2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
  int i3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
  int j1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
  int j2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
  int j3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
  int k1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
  int k2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
  int k3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
  int l1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
  int l2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
  int l3 = SIMPLEX_4D[c] >= 1 ? 1 : 0;

  WN_DECIMAL x1 = x0 - i1 + G4;
  WN_DECIMAL y1 = y0 - j1 + G4;
  WN_DECIMAL z1 = z0 - k1 + G4;
  WN_DECIMAL w1 = w0 - l1 + G4;
  WN_DECIMAL x2 = x0 - i2 + 2*G4;
  WN_DECIMAL y2 = y0 - j2 + 2*G4;
  WN_DECIMAL z2 = z0 - k2 + 2*G4;
  WN_DECIMAL w2 = w0 - l2 + 2*G4;
  WN_DECIMAL x3 = x0 - i3 + 3*G4;
  WN_DECIMAL y3 = y0 - j3 + 3*G4;
  WN_DECIMAL z3 = z0 - k3 + 3*G4;
  WN_DECIMAL w3 = w0 - l3 + 3*G4;
  WN_DECIMAL x4 = x0 - 1 + 4*G4;
  WN_DECIMAL y4 = y0 - 1 + 4*G4;
  WN_DECIMAL z4 = z0 - 1 + 4*G4;
  WN_DECIMAL w4 = w0 - 1 + 4*G4;

  t = static_cast<WN_DECIMAL>(0.6) - x0*x0 - y0*y0 - z0*z0 - w0*w0;
  if (t < 0) n0 = 0;
  else
  {
    t *= t;
    n0 = t * t * GradCoord4D(offset, i, j, k, l, x0, y0, z0, w0);
  }

  t = static_cast<WN_DECIMAL>(0.6) - x1*x1 - y1*y1 - z1*z1 - w1*w1;
  if (t < 0) n1 = 0;
  else
  {
    t *= t;
    n1 = t * t * GradCoord4D(offset, i + i1, j + j1, k + k1, l + l1, x1, y1, z1, w1);
  }

  t = static_cast<WN_DECIMAL>(0.6) - x2*x2 - y2*y2 - z2*z2 - w2*w2;
  if (t < 0) n2 = 0;
  else
  {
    t *= t;
    n2 = t * t * GradCoord4D(offset, i + i2, j + j2, k + k2, l + l2, x2, y2, z2, w2);
  }

  t = static_cast<WN_DECIMAL>(0.6) - x3*x3 - y3*y3 - z3*z3 - w3*w3;
  if (t < 0) n3 = 0;
  else
  {
    t *= t;
    n3 = t * t * GradCoord4D(offset, i + i3, j + j3, k + k3, l + l3, x3, y3, z3, w3);
  }

  t = static_cast<WN_DECIMAL>(0.6) - x4*x4 - y4*y4 - z4*z4 - w4*w4;
  if (t < 0) n4 = 0;
  else
  {
    t *= t;
    n4 = t * t * GradCoord4D(offset, i + 1, j + 1, k + 1, l + 1, x4, y4, z4, w4);
  }

  return 27 * (n0 + n1 + n2 + n3 + n4);
}

#ifdef WN_INCLUDE_SIMPLEX_FRACTAL
// 2D Simplex Fractal Functions
WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalFBM(WN_DECIMAL x, WN_DECIMAL y)
{
  return SingleFractalFBM<Single2DFPtr>(&WasmNoise::SingleSimplex, x, y);
}

WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalBillow(WN_DECIMAL x, WN_DECIMAL y)
{
  return SingleFractalBillow<Single2DFPtr>(&WasmNoise::SingleSimplex, x, y);
}

WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y)
{
  return SingleFractalRidgedMulti<Single2DFPtr>(&WasmNoise::SingleSimplex, x, y);
}

// 3D Simplex Fractal Functions
WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalFBM(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  return SingleFractalFBM<Single3DFPtr>(&WasmNoise::SingleSimplex, x, y, z);
}

WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalBillow(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  return SingleFractalBillow<Single3DFPtr>(&WasmNoise::SingleSimplex, x, y, z);
}

WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  return SingleFractalRidgedMulti<Single3DFPtr>(&WasmNoise::SingleSimplex, x, y, z);
}

// 4D Simplex Fractal Functions
WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalFBM(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w)
{
  return SingleFractalFBM<Single4DFPtr>(&WasmNoise::SingleSimplex, x, y, z, w);
}

WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalBillow(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w)
{
  return SingleFractalBillow<Single4DFPtr>(&WasmNoise::SingleSimplex, x, y, z, w);
}

WN_INLINE WN_DECIMAL WasmNoise::SingleSimplexFractalRidgedMulti(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w)
{
  return SingleFractalRidgedMulti<Single4DFPtr>(&WasmNoise::SingleSimplex, x, y, z, w);
}
#endif // WN_INCLUDE_SIMPLEX_FRACTAL

// End Single Noise Function Section ***************************

// Public Simplex Noise Functions

#ifdef WN_INCLUDE_SIMPLEX
// 2D Single
WN_INLINE WN_DECIMAL WasmNoise::GetSimplex(WN_DECIMAL x, WN_DECIMAL y) const
{
  return SingleSimplex(x * frequency, y * frequency);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction)
{
  return GetStrip2D(&WasmNoise::SinglePerlin, startX, startY, length, direction);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height)
{
  return GetSquare2D(&WasmNoise::SingleSimplex, startX, startY, width, height);
}

// 3D Single
WN_INLINE WN_DECIMAL WasmNoise::GetSimplex(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) const
{
  return SingleSimplex(x * frequency, y * frequency, z * frequency);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction)
{
  return GetStrip3D(&WasmNoise::SingleSimplex, startX, startY, startZ, length, direction);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane)
{
  return GetSquare3D(&WasmNoise::SingleSimplex, startX, startY, startZ, width, height, plane);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth)
{
  return GetCube3D(&WasmNoise::SingleSimplex, startX, startY, startZ, width, height, depth);
}

// 4D Single
WN_INLINE WN_DECIMAL WasmNoise::GetSimplex(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w) const
{
  return SingleSimplex(x * frequency, y * frequency, z * frequency, w * frequency);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 length, StripDirection direction)
{
  return GetStrip4D(&WasmNoise::SingleSimplex, startX, startY, startZ, startW, length, direction);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, SquarePlane plane)
{
  return GetSquare4D(&WasmNoise::SingleSimplex, startX, startY, startZ, startW, width, height, plane);
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, uint32 depth)
{
  return GetCube4D(&WasmNoise::SingleSimplex, startX, startY, startZ, startW, width, height, depth);
}
#endif // WN_INCLUDE_SIMPLEX

#ifdef WN_INCLUDE_SIMPLEX_FRACTAL
// 2D Fractal
WN_INLINE WN_DECIMAL WasmNoise::GetSimplexFractal(WN_DECIMAL x, WN_DECIMAL y)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return SingleSimplexFractalFBM(x * frequency, y * frequency);
  case FractalType::Billow:       return SingleSimplexFractalBillow(x * frequency, y * frequency);
  case FractalType::RidgedMulti:  return SingleSimplexFractalRidgedMulti(x * frequency, y * frequency);
  default:
    ABORT();
    return 0;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetStrip2D(&WasmNoise::SingleSimplexFractalFBM, startX, startY, length, direction);
  case FractalType::Billow:       return GetStrip2D(&WasmNoise::SingleSimplexFractalBillow, startX, startY, length, direction);
  case FractalType::RidgedMulti:  return GetStrip2D(&WasmNoise::SingleSimplexFractalRidgedMulti, startX, startY, length, direction);
  default:
    ABORT();
    return nullptr;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetSquare2D(&WasmNoise::SingleSimplexFractalFBM, startX, startY, width, height);
  case FractalType::Billow:       return GetSquare2D(&WasmNoise::SingleSimplexFractalBillow, startX, startY, width, height);
  case FractalType::RidgedMulti:  return GetSquare2D(&WasmNoise::SingleSimplexFractalRidgedMulti, startX, startY, width, height);
  default:
    ABORT();
    return nullptr;
  }
}

// 3D Fractal
WN_INLINE WN_DECIMAL WasmNoise::GetSimplexFractal(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return SingleSimplexFractalFBM(x * frequency, y * frequency, z * frequency);
  case FractalType::Billow:       return SingleSimplexFractalBillow(x * frequency, y * frequency, z * frequency);
  case FractalType::RidgedMulti:  return SingleSimplexFractalRidgedMulti(x * frequency, y * frequency, z * frequency);
  default:
    ABORT();
    return 0;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetStrip3D(&WasmNoise::SingleSimplexFractalFBM, startX, startY, startZ, length, direction);
  case FractalType::Billow:       return GetStrip3D(&WasmNoise::SingleSimplexFractalBillow, startX, startY, startZ, length, direction);
  case FractalType::RidgedMulti:  return GetStrip3D(&WasmNoise::SingleSimplexFractalRidgedMulti, startX, startY, startZ, length, direction);
  default:
    ABORT();
    return nullptr;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetSquare3D(&WasmNoise::SingleSimplexFractalFBM, startX, startY, startZ, width, height, plane);
  case FractalType::Billow:       return GetSquare3D(&WasmNoise::SingleSimplexFractalBillow, startX, startY, startZ, width, height, plane);
  case FractalType::RidgedMulti:  return GetSquare3D(&WasmNoise::SingleSimplexFractalRidgedMulti, startX, startY, startZ, width, height, plane);
  default:
    ABORT();
    return nullptr;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexFractalCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetCube3D(&WasmNoise::SingleSimplexFractalFBM, startX, startY, startZ, width, height, depth);
  case FractalType::Billow:       return GetCube3D(&WasmNoise::SingleSimplexFractalBillow, startX, startY, startZ, width, height, depth);
  case FractalType::RidgedMulti:  return GetCube3D(&WasmNoise::SingleSimplexFractalRidgedMulti, startX, startY, startZ, width, height, depth);
  default:
    ABORT();
    return nullptr;
  }
}

// 4D Fractal
WN_INLINE WN_DECIMAL WasmNoise::GetSimplexFractal(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return SingleSimplexFractalFBM(x * frequency, y * frequency, z * frequency, w * frequency);
  case FractalType::Billow:       return SingleSimplexFractalBillow(x * frequency, y * frequency, z * frequency, w * frequency);
  case FractalType::RidgedMulti:  return SingleSimplexFractalRidgedMulti(x * frequency, y * frequency, z * frequency, w * frequency);
  default:
    ABORT();
    return 0;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexFractalStrip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 length, StripDirection direction)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetStrip4D(&WasmNoise::SingleSimplexFractalFBM, startX, startY, startZ, startW, length, direction);
  case FractalType::Billow:       return GetStrip4D(&WasmNoise::SingleSimplexFractalBillow, startX, startY, startZ, startW, length, direction);
  case FractalType::RidgedMulti:  return GetStrip4D(&WasmNoise::SingleSimplexFractalRidgedMulti, startX, startY, startZ, startW, length, direction);
  default:
    ABORT();
    return nullptr;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexFractalSquare(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, SquarePlane plane)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetSquare4D(&WasmNoise::SingleSimplexFractalFBM, startX, startY, startZ, startW, width, height, plane);
  case FractalType::Billow:       return GetSquare4D(&WasmNoise::SingleSimplexFractalBillow, startX, startY, startZ, startW, width, height, plane);
  case FractalType::RidgedMulti:  return GetSquare4D(&WasmNoise::SingleSimplexFractalRidgedMulti, startX, startY, startZ, startW, width, height, plane);
  default:
    ABORT();
    return nullptr;
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSimplexFractalCube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, uint32 depth)
{
  switch(fractalType)
  {
  case FractalType::FBM:          return GetCube4D(&WasmNoise::SingleSimplexFractalFBM, startX, startY, startZ, startW, width, height, depth);
  case FractalType::Billow:       return GetCube4D(&WasmNoise::SingleSimplexFractalBillow, startX, startY, startZ, startW, width, height, depth);
  case FractalType::RidgedMulti:  return GetCube4D(&WasmNoise::SingleSimplexFractalRidgedMulti, startX, startY, startZ, startW, width, height, depth);
  default:
    ABORT();
    return nullptr;
  }
}
#endif // WN_INCLUDE_SIMPLEX_FRACTL
