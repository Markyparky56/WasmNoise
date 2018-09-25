#pragma once
#include "WasmNoise.hpp"
#include "WasmNoise.Common.hpp"
#include "invoke.hpp"

// Fractal Functions
template<class NoiseFunc, class... Args> WN_INLINE WN_DECIMAL WasmNoise::SingleFractalFBM(NoiseFunc func, Args... args)
{
  WN_DECIMAL sum = invoke(func, *this, perm[0], args...);
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    ((args *= fractalLacunarity), ...);

    amp *= fractalGain;
    sum += invoke(func, *this, perm[i], args...) * amp;
  }

  return sum * fractalBounding;
}

template<class NoiseFunc, class... Args> WN_INLINE WN_DECIMAL WasmNoise::SingleFractalBillow(NoiseFunc func, Args... args)
{
  WN_DECIMAL sum = FastAbs(invoke(func, *this, perm[0], args...)) * 2 - 1;
  WN_DECIMAL amp = 1;
  uint32 i = 0;

  while(++i < fractalOctaves)
  {
    ((args *= fractalLacunarity), ...);

    amp *= fractalGain;
    sum += (FastAbs(invoke(func, *this, perm[i], args...)) * 2 - 1) * amp;
  }

  return sum * fractalBounding;
}

template<class NoiseFunc, class... Args> WN_INLINE WN_DECIMAL WasmNoise::SingleFractalRidgedMulti(NoiseFunc func, Args... args)
{
  WN_DECIMAL signal = 1 - FastAbs(invoke(func, *this, perm[0], args...));
  signal *= signal;
  WN_DECIMAL sum = signal * fractalExponents[0];
  WN_DECIMAL weight = signal * fractalGain;
  Clamp(weight, 0.0, 1.0);
  uint32 i = 0; 

  while(++i < fractalOctaves)
  {
    ((args *= fractalLacunarity), ...);

    signal = 1 - FastAbs(invoke(func, *this, perm[i], args...));
    signal *= signal;
    signal *= weight;
    weight = signal * fractalGain;
    Clamp(weight, 0.0, 1.0);

    sum += (signal * fractalExponents[i]);
  }

  return (sum * WN_DECIMAL(1.25)) - WN_DECIMAL(1.0);
}
