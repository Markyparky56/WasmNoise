#pragma once
#include "WasmNoise.hpp"
#include "WasmNoise.Common.hpp"
#include "invoke.hpp"

// Fractal Functions
template<class NoiseFunc, class... Args> WN_INLINE WN_DECIMAL WasmNoise::SingleFractalFBM(NoiseFunc func, Args... args)
{
  //WN_DECIMAL sum = (this->*func)(perm[0], args...);
  WN_DECIMAL sum = invoke(func, *this, args..., perm[0]);
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    ((args *= fractalLacunarity), ...);

    amp *= fractalGain;
    sum += invoke(func, *this, args..., perm[i]) * amp;
  }

  return sum * fractalBounding;
}

template<class NoiseFunc, class... Args> WN_INLINE WN_DECIMAL WasmNoise::SingleFractalBillow(NoiseFunc func, Args... args)
{
  WN_DECIMAL sum = FastAbs(invoke(func, *this, args..., perm[0])) * 2 - 1;
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    ((args *= fractalLacunarity), ...);

    amp *= fractalGain;
    sum += (FastAbs(invoke(func, *this, args..., perm[i])) * 2 - 1) * amp;
  }

  return sum * fractalBounding;
}

template<class NoiseFunc, class... Args> WN_INLINE WN_DECIMAL WasmNoise::SingleFractalRidgedMulti(NoiseFunc func, Args... args)
{
  WN_DECIMAL sum = 1 - FastAbs(invoke(func, *this, args..., perm[0]));
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    ((args *= fractalLacunarity), ...);

    amp *= fractalGain;
    sum += (1 - FastAbs(invoke(func, *this, args..., perm[i]))) * amp;
  }

  return sum;
}
