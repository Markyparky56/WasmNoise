#include "WasmNoise.hpp"

static WasmNoise wasmNoise;

extern "C"
{
  void SetSeed(int32 _seed) { wasmNoise.SetSeed(_seed); }
  int32 GetSeed() { return wasmNoise.GetSeed(); }

  WM_DECIMAL GetPerlin(WM_DECIMAL x, WM_DECIMAL y) { return wasmNoise.GetPerlin(x, y); }
}
