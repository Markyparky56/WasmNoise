#include "WasmNoise.hpp"

static WasmNoise wasmNoise;

extern "C"
{
  void Initialise() { wasmNoise.Initialise(); }

  void SetSeed(int32 _seed) { wasmNoise.SetSeed(_seed); }
  int32 GetSeed() { return wasmNoise.GetSeed(); }

  void SetFrequency(WM_DECIMAL _frequency) { wasmNoise.SetFrequency(_frequency); }
  WM_DECIMAL GetFrequency() { return wasmNoise.GetFrequency(); }

  WM_INLINE WM_DECIMAL GetPerlin(WM_DECIMAL x, WM_DECIMAL y) 
  {
    return wasmNoise.GetPerlin(x, y);
  }

  WM_INLINE WM_DECIMAL GetPerlin_log(WM_DECIMAL x, WM_DECIMAL y)
  {
    consolelogDecimal(x);
    consolelogDecimal(y);
    WM_DECIMAL val = wasmNoise.GetPerlin_log(x, y); 
    consolelogDecimal(val);
    return val;
  }
}
