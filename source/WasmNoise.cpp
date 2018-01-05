#include "WasmNoise.hpp"
#include "WasmNoise.Common.hpp"
#include "xoroshiro128plus.hpp"
#include "uniform_int_distribution.hpp"

#if defined(WN_INCLUDE_PERLIN) || defined(WN_INCLUDE_PERLIN_FRACTAL)
#include "WasmNoise.Perlin.hpp"
#endif // WN_INCLUDE_PERLIN || WN_INCLUDE_PERLIN_FRACTAL

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
  return perm12[(x & 0xff) + perm[(y & 0xff) + perm[(z &0xff) + offset]]];
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

// Base Array Functions
// 2D - Single
WN_INLINE WN_DECIMAL *WasmNoise::GetStrip2D(Single2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction) 
{
  WN_DECIMAL *values = returnHelper.NewArray(length);  
  switch(direction)
  {
  case StripDirection::XAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)(0, (startX+i) * frequency, startY * frequency);
    }
    return values;
  }
  case StripDirection::YAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)(0, startX * frequency, (startY+i) * frequency);
    }
    return values;
  }
  default: // Invalid direction in 2D Space
    ABORT();
    // Return a null value in case aborts are disabled
    return nullptr;
  }  
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSquare2D(Single2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height);
  for(uint32 y = 0; y < height; y++)
  {
    for(uint32 x = 0; x < width; x++)
    {
      values[(width * y) + x] = (this->*noiseFunc)(0, (startX+x) * frequency, (startY+y) * frequency);
    }
  }
  return values;
}

// 2D - Fractal
WN_INLINE WN_DECIMAL *WasmNoise::GetStrip2D(Fractal2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, StripDirection direction) 
{
  WN_DECIMAL *values = returnHelper.NewArray(length);  
  switch(direction)
  {
  case StripDirection::XAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)((startX+i) * frequency, startY * frequency);
    }
    return values;
  }
  case StripDirection::YAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)(startX * frequency, (startY+i) * frequency);
    }
    return values;
  }
  default: // Invalid direction in 2D Space
    ABORT();
    // Return a null value in case aborts are disabled
    return nullptr;
  }  
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSquare2D(Fractal2DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height);
  for(uint32 y = 0; y < height; y++)
  {
    for(uint32 x = 0; x < width; x++)
    {
      values[(width * y) + x] = (this->*noiseFunc)((startX+x) * frequency, (startY+y) * frequency);
    }
  }
  return values;
}

// 3D - Single
WN_INLINE WN_DECIMAL *WasmNoise::GetStrip3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction)
{
  WN_DECIMAL *values = returnHelper.NewArray(length);
  switch(direction)
  {
  case StripDirection::XAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)(0, (startX + i) * frequency, startY *frequency, startZ * frequency);
    }
    return values;
  }
  case StripDirection::YAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)(0, startX * frequency, (startY + i) *frequency, startZ * frequency);
    }
    return values;
  }
  case StripDirection::ZAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)(0, startX * frequency, startY *frequency, (startZ + i) * frequency);
    }
    return values;
  }
  default:
    ABORT();
    return nullptr;
  }  
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSquare3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height);
  switch(plane)
  {
  case SquarePlane::XYPlane:
  {
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width * y) + x] = (this->*noiseFunc)(0, (startX+x) * frequency, (startY+y) * frequency, startZ * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::XZPlane:
  {
    for(uint32 z = 0; z < height; z++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width * z) + x] = (this->*noiseFunc)(0, (startX+x) * frequency, startY * frequency, (startZ+z) * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::ZYPlane:
  {
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 z = 0; z < width; z++)
      {
        values[(width * y) + z] = (this->*noiseFunc)(0, startX * frequency, (startY+y) * frequency, (startZ+z) * frequency);
      }
    }
    return values;  
  }
  default:
    ABORT();
    return nullptr;  
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetCube3D(Single3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height*depth);
  for(uint32 z = 0; z < depth; z++)
  {
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(height * width * z) + (width * y) + x] = (this->*noiseFunc)(0, (startX+x) * frequency, (startY+y) * frequency, (startZ+z) * frequency);
      }
    }
  }
  return values;
}

// 3D - Fractal
WN_INLINE WN_DECIMAL *WasmNoise::GetStrip3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, StripDirection direction)
{
  WN_DECIMAL *values = returnHelper.NewArray(length);
  switch(direction)
  {
  case StripDirection::XAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)((startX + i) * frequency, startY *frequency, startZ * frequency);
    }
    return values;
  }
  case StripDirection::YAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)(startX * frequency, (startY + i) *frequency, startZ * frequency);
    }
    return values;
  }
  case StripDirection::ZAxis:
  {
    for(uint32 i = 0; i < length; i++)
    {
      values[i] = (this->*noiseFunc)(startX * frequency, startY *frequency, (startZ + i) * frequency);
    }
    return values;
  }
  default:
    ABORT();
    return nullptr;
  }  
}

WN_INLINE WN_DECIMAL *WasmNoise::GetSquare3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, SquarePlane plane)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height);
  switch(plane)
  {
  case SquarePlane::XYPlane:
  {
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width * y) + x] = (this->*noiseFunc)((startX+x) * frequency, (startY+y) * frequency, startZ * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::XZPlane:
  {
    for(uint32 z = 0; z < height; z++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(width * z) + x] = (this->*noiseFunc)((startX+x) * frequency, startY * frequency, (startZ+z) * frequency);
      }
    }
    return values;  
  }
  case SquarePlane::ZYPlane:
  {
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 z = 0; z < width; z++)
      {
        values[(width * y) + z] = (this->*noiseFunc)(startX * frequency, (startY+y) * frequency, (startZ+z) * frequency);
      }
    }
    return values;  
  }
  default:
    ABORT();
    return nullptr;  
  }
}

WN_INLINE WN_DECIMAL *WasmNoise::GetCube3D(Fractal3DFPtr noiseFunc, WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth)
{
  WN_DECIMAL *values = returnHelper.NewArray(width*height*depth);
  for(uint32 z = 0; z < depth; z++)
  {
    for(uint32 y = 0; y < height; y++)
    {
      for(uint32 x = 0; x < width; x++)
      {
        values[(height * width * z) + (width * y) + x] = (this->*noiseFunc)((startX+x) * frequency, (startY+y) * frequency, (startZ+z) * frequency);
      }
    }
  }
  return values;
}
