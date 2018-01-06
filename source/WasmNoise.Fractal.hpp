#pragma once
#include "WasmNoise.hpp"
#include "WasmNoise.Common.hpp"

// 2D Fractal Functions
WN_INLINE WN_DECIMAL WasmNoise::SingleFractalFBM(Single2DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y)
{
  WN_DECIMAL sum = (this->*noiseFunc)(perm[0], x, y);
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;

    amp *= fractalGain;
    sum += (this->*noiseFunc)(perm[i], x, y) * amp;
  }

  return sum * fractalBounding;
}

WN_INLINE WN_DECIMAL WasmNoise::SingleFractalBillow(Single2DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y)
{
  WN_DECIMAL sum = FastAbs((this->*noiseFunc)(perm[0], x, y)) * 2 - 1;
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;
  
    amp *= fractalGain;
    sum += (FastAbs((this->*noiseFunc)(perm[i], x, y)) * 2 - 1) * amp;
  }

  return sum * fractalBounding;
}

WN_INLINE WN_DECIMAL WasmNoise::SingleFractalRidgedMulti(Single2DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y)
{
  WN_DECIMAL sum = 1 - FastAbs((this->*noiseFunc)(perm[0], x, y));
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;

    amp *= fractalGain;
    sum -= (1 - FastAbs((this->*noiseFunc)(perm[i], x, y))) * amp;
  }

  return sum;
}

// 3D Fractal Functions
WN_INLINE WN_DECIMAL WasmNoise::SingleFractalFBM(Single3DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  WN_DECIMAL sum = (this->*noiseFunc)(perm[0], x, y, z);
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;
    z *= fractalLacunarity;

    amp *= fractalGain;
    sum += (this->*noiseFunc)(perm[i], x, y, z) * amp;
  }

  return sum * fractalBounding;
}

WN_INLINE WN_DECIMAL WasmNoise::SingleFractalBillow(Single3DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  WN_DECIMAL sum = FastAbs((this->*noiseFunc)(perm[0], x, y, z)) * 2 - 1;
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;
    z *= fractalLacunarity;
  
    amp *= fractalGain;
    sum += (FastAbs((this->*noiseFunc)(perm[i], x, y, z)) * 2 - 1) * amp;
  }

  return sum * fractalBounding;
}

WN_INLINE WN_DECIMAL WasmNoise::SingleFractalRidgedMulti(Single3DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z)
{
  WN_DECIMAL sum = 1 - FastAbs((this->*noiseFunc)(perm[0], x, y, z));
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;
    z *= fractalLacunarity;

    amp *= fractalGain;
    sum -= (1 - FastAbs((this->*noiseFunc)(perm[i], x, y, z))) * amp;
  }

  return sum;
}

// 4D Fractal Functions
WN_INLINE WN_DECIMAL WasmNoise::SingleFractalFBM(Single4DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w)
{
  WN_DECIMAL sum = (this->*noiseFunc)(perm[0], x, y, z, w);
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;
    z *= fractalLacunarity;
    w *= fractalLacunarity;

    amp *= fractalGain;
    sum += (this->*noiseFunc)(perm[i], x, y, z, w) * amp;
  }

  return sum * fractalBounding;
}

WN_INLINE WN_DECIMAL WasmNoise::SingleFractalBillow(Single4DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w)
{
  WN_DECIMAL sum = FastAbs((this->*noiseFunc)(perm[0], x, y, z, w)) * 2 - 1;
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;
    z *= fractalLacunarity;
    w *= fractalLacunarity;
  
    amp *= fractalGain;
    sum += (FastAbs((this->*noiseFunc)(perm[i], x, y, z, w)) * 2 - 1) * amp;
  }

  return sum * fractalBounding;
}

WN_INLINE WN_DECIMAL WasmNoise::SingleFractalRidgedMulti(Single4DFPtr noiseFunc, WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w)
{
  WN_DECIMAL sum = 1 - FastAbs((this->*noiseFunc)(perm[0], x, y, z, w));
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    x *= fractalLacunarity;
    y *= fractalLacunarity;
    z *= fractalLacunarity;
    w *= fractalLacunarity;

    amp *= fractalGain;
    sum -= (1 - FastAbs((this->*noiseFunc)(perm[i], x, y, z, w))) * amp;
  }

  return sum;
}
