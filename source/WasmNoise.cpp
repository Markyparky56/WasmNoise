#include "WasmNoise.hpp"
#include "WasmNoise.Common.hpp"
#include "xoroshiro128plus.hpp"
#include "uniform_int_distribution.hpp"
#include "invoke.hpp"

#if defined(WN_INCLUDE_PERLIN) || defined(WN_INCLUDE_PERLIN_FRACTAL)
#include "WasmNoise.Perlin.hpp"
#endif // WN_INCLUDE_PERLIN || WN_INCLUDE_PERLIN_FRACTAL
#if defined(WN_INCLUDE_SIMPLEX) || defined(WN_INCLUDE_SIMPLEX_FRACTAL)
#include "WasmNoise.Simplex.hpp"
#endif // WN_INCLUDE_SIMPLEX || WN_INCLUDE_SIMPLEX_FRACTAL
#if defined(WN_INCLUDE_CELLULAR) || defined(WN_INCLUDE_CELLULAR_FRACTAL)
#include "WasmNoise.Cellular.hpp"
#endif // WN_INCLUDE_CELLULAR

void WasmNoise::SetSeed(int32 _seed)
{
  seed = _seed;

  xoroshiro128plus gen(seed); 

  for(int32 i = 0; i < 256; i++)
  {
    perm[i] = i;
  }

  for(int32 j = 0; j < 256; j++)
  {
    uniform_int_distribution<> dist(0, 256-j);
    int k = dist(gen) + j;
    int l = perm[j];
    perm[j] = perm[j + 256] = perm[k];
    perm[k] = l;
    perm12[j] = perm12[j+256] = perm[j] % 12;
  }
}

void WasmNoise::CalculateFractalBounding()
{
  WN_DECIMAL amp = fractalGain;
  WN_DECIMAL ampFractal = WN_DECIMAL(1.0);
  for(uint32 i = 1; i < fractalOctaves; i++)
  {
    ampFractal += amp;
    amp *= fractalGain;
  }
  fractalBounding = WN_DECIMAL(1.0) / ampFractal;
}

// Index Functions
WN_INLINE uint8 WasmNoise::Index2D_12(uint8 offset, int32 x, int32 y) const
{
  return perm12[(x & 0xff) + perm[(y & 0xff) + offset]];
}

WN_INLINE uint8 WasmNoise::Index3D_12(uint8 offset, int32 x, int32 y, int32 z) const
{
  return perm12[(x & 0xff) + perm[(y & 0xff) + perm[(z & 0xff) + offset]]];
}

WN_INLINE uint8 WasmNoise::Index4D_32(uint8 offset, int32 x, int32 y, int32 z, int32 w) const
{
  return perm[(x & 0xff) + perm[(y & 0xff) + perm[(z & 0xff) + perm[(w & 0xff) + offset]]]] & 31;
}

WN_INLINE uint8 WasmNoise::Index2D_256(uint8 offset, int32 x, int32 y) const
{
  return perm[(x & 0xff) + perm[(y & 0xff) + offset]];
}

WN_INLINE uint8 WasmNoise::Index3D_256(uint8 offset, int32 x, int32 y, int32 z) const
{
  return perm[(x & 0xff) + perm[(y & 0xff) + perm[(z & 0xff) + offset]]];
}

WN_INLINE uint8 WasmNoise::Index4D_256(uint8 offset, int32 x, int32 y, int32 z, int32 w) const
{
  return perm[(x & 0xff) + perm[(y & 0xff) + perm[(z & 0xff) + perm[(w & 0xff) + offset]]]];
}

// Gradient Coordinate Functions
WN_INLINE WN_DECIMAL WasmNoise::GradCoord2D(uint8 offset, int32 x, int32 y, WN_DECIMAL xd, WN_DECIMAL yd) const
{
  uint8 lutPos = Index2D_12(offset, x, y);
  return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos];
}

WN_INLINE WN_DECIMAL WasmNoise::GradCoord3D(uint8 offset, int32 x, int32 y, int32 z, WN_DECIMAL xd, WN_DECIMAL yd, WN_DECIMAL zd) const
{
  uint8 lutPos = Index3D_12(offset, x, y, z);
  return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos] + zd*GRAD_Z[lutPos];
}

WN_INLINE WN_DECIMAL WasmNoise::GradCoord4D(uint8 offset, int32 x, int32 y, int32 z, int32 w, WN_DECIMAL xd, WN_DECIMAL yd, WN_DECIMAL zd, WN_DECIMAL wd) const
{
  uint8 lutPos = Index4D_32(offset, x, y, z, w) << 2;
  return xd*GRAD_4D[lutPos] + yd*GRAD_4D[lutPos + 1] + zd*GRAD_4D[lutPos + 2] + wd*GRAD_4D[lutPos + 3];
}

// Base Array Functions

// 2D Strip
template<class NoiseFunc> 
WN_INLINE WN_DECIMAL *WasmNoise::GetStrip(NoiseFunc func, uint32 length, StripDirection direction, WN_DECIMAL startX, WN_DECIMAL startY)
{
  switch(direction)
  {
  case StripDirection::XAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, (startX+i) * frequency, startY * frequency);
    }
    return values;
  }
  case StripDirection::YAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, startX * frequency, (startY+i) * frequency);
    }
    return values;
  }
  default: // Z and W are invalid directions in 2D-space
    ABORT();
    return nullptr; // ABORT is likely disabled so return a nullptr
  }
}

// 3D Strip
template<class NoiseFunc> 
WN_INLINE WN_DECIMAL *WasmNoise::GetStrip(NoiseFunc func, uint32 length, StripDirection direction, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ)
{
  switch(direction)
  {
  case StripDirection::XAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, (startX+i) * frequency, startY * frequency, startZ * frequency);
    }
    return values;
  }
  case StripDirection::YAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);    
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, startX * frequency, (startY+i) * frequency, startZ * frequency);
    }
    return values;
  }
  case StripDirection::ZAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);    
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, startX * frequency, startY * frequency, (startZ+i) * frequency);
    }
    return values;
  }
  default: // W is an invalid direction in 3D-space
    ABORT();
    return nullptr;
  }
}

// 4D Strip
template<class NoiseFunc>
WN_INLINE WN_DECIMAL *WasmNoise::GetStrip(NoiseFunc func, uint32 length, StripDirection direction, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW)
{
  switch(direction)
  {
  case StripDirection::XAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, (startX+i) * frequency, startY * frequency, startZ * frequency, startW * frequency);
    }
    return values;
  }
  case StripDirection::YAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, startX * frequency, (startY+i) * frequency, startZ * frequency, startW * frequency);
    }
    return values;
  }
  case StripDirection::ZAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, startX * frequency, startY * frequency, (startZ+i) * frequency, startW * frequency);
    }
    return values;
  }
  case StripDirection::WAxis:
  {
    WN_DECIMAL *values = returnHelper.NewArray(length);
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = invoke(func, *this, startX * frequency, startY * frequency, startZ * frequency, (startW+i) * frequency);
    }
    return values;
  }
  default: // Should never happen unless someone is sending non enum values
    ABORT();
    return nullptr;
  }  
}

// 2D Square
template<class NoiseFunc> 
WN_INLINE WN_DECIMAL *WasmNoise::GetSquare(NoiseFunc func, uint32 width, uint32 height, WN_DECIMAL startX, WN_DECIMAL startY)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height);
  for(uint32 y = 0; y < height; y++)
  {
    for(uint32 x = 0; x < width; x++)
    {
      values[(width*y) + x] = invoke(func, *this, (startX+x) * frequency, (startY+y) * frequency);
    }
  }
  return values;
}

// 3D Square
template<class NoiseFunc> 
WN_INLINE WN_DECIMAL *WasmNoise::GetSquare(NoiseFunc func, uint32 width, uint32 height, SquarePlane plane, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ)
{
  switch(plane)
  {
  case SquarePlane::XYPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width*y) + x] = invoke(func, *this, (startX+x) * frequency, (startY+y) * frequency, startZ * frequency);
      }
    }
    return values;
  }
  case SquarePlane::XZPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);
    for(uint32 z = 0; z < height; z++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width*z) + x] = invoke(func, *this, (startX+x) * frequency, startY * frequency, (startZ+z) * frequency);
      }
    }
    return values;
  }
  case SquarePlane::ZYPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 z = 0; z < width; z++)
      {
        values[(width*y) + z] = invoke(func, *this, startX * frequency, (startY+y) * frequency, (startZ+z) * frequency);
      }
    }
    return values;
  }
  default: // W-planes are invalid in 3D-space
    ABORT();
    return nullptr;
  }
}

// 4D Square
template<class NoiseFunc> 
WN_INLINE WN_DECIMAL *WasmNoise::GetSquare(NoiseFunc func, uint32 width, uint32 height, SquarePlane plane, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW)
{
  switch(plane)
  {
  case SquarePlane::XYPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);    
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width * y) + x] = invoke(func, *this, (startX+x) * frequency, (startY+y) * frequency, startZ * frequency, startW * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::XZPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);        
    for(uint32 z = 0; z < height; z++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width * z) + x] = invoke(func, *this, (startX+x) * frequency, startY * frequency, (startZ+z) * frequency, startW * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::ZYPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);        
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 z = 0; z < width; z++)
      {
        values[(width * y) + z] = invoke(func, *this, startX * frequency, (startY+y) * frequency, (startZ+z) * frequency, startW * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::XWPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);        
    for(uint32 w = 0; w < height; w++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width * w) + x] = invoke(func, *this, (startX+x) * frequency, startY * frequency, startZ * frequency, (startW+w) * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::YWPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);        
    for(uint32 w = 0; w < height; w++)
    {
      for(uint32 y = 0; y < width; y++)
      {
        values[(width * w) + y] = invoke(func, *this, startX * frequency, (startY+y) * frequency, startZ * frequency, (startW+w) * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::ZWPlane:
  {
    WN_DECIMAL *values = returnHelper.NewArray(width*height);        
    for(uint32 w = 0; w < height; w++)
    {
      for(uint32 z = 0; z < width; z++)
      {
        values[(width * w) + z] = invoke(func, *this, startX * frequency, startY * frequency, (startZ+z) * frequency, (startW+w) * frequency);
      }
    }
    return values;  
  }
  default:
    ABORT();
    return nullptr;  
  }
}

// 3D Cube
template<class NoiseFunc> 
WN_INLINE WN_DECIMAL *WasmNoise::GetCube(NoiseFunc func, uint32 width, uint32 height, uint32 depth, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height*depth);
  for(uint32 z = 0; z < depth; z++)
  {
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 x = 0; x < height; x++)
      {
        values[(height * width * z) + (width * y) + x] = invoke(func, *this, (startX+x) * frequency, (startY+y) * frequency, (startZ+z) * frequency);
      }
    }
  }
  return values;
}

// 4D Cube
template<class NoiseFunc> 
WN_INLINE WN_DECIMAL *WasmNoise::GetCube(NoiseFunc func, uint32 width, uint32 height, uint32 depth, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height*depth);
  for(uint32 z = 0; z < depth; z++)
  {
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 x = 0; x < height; x++)
      {
        values[(height * width * z) + (width * y) + x] = invoke(func, *this, (startX+x) * frequency, (startY+y) * frequency, (startZ+z) * frequency, startW * frequency);
      }
    }
  }
  return values;
}
