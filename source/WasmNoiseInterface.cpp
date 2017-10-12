#include "WasmNoise.hpp"
#include <stdlib.h> // Included from libc, allows us to import malloc and free

extern "C"
{
  void consolelogi(size_t outI);
}

// We need to override the global new/delete operators to use our own malloc/free functions
// which allocate and free memory in the WebAssembly modules linear memory
void *operator new     (size_t size) { return malloc(size); }
void *operator new[]   (size_t size) { return malloc(size); }
void  operator delete  (void   *ptr) noexcept { return free(ptr); }
void  operator delete[](void   *ptr) noexcept { return free(ptr); }

static WasmNoise wasmNoise;

extern "C"
{  
  void SetSeed(int32 _seed) { wasmNoise.SetSeed(_seed); }
  int32 GetSeed() { return wasmNoise.GetSeed(); }

  void SetFrequency(WM_DECIMAL _frequency) { wasmNoise.SetFrequency(_frequency); }
  WM_DECIMAL GetFrequency() { return wasmNoise.GetFrequency(); }

  WM_INLINE WM_DECIMAL GetPerlin(WM_DECIMAL x, WM_DECIMAL y) 
  {
    return wasmNoise.GetPerlin(x, y);
  }

  WM_INLINE WM_DECIMAL *GetPerlinStrip(WM_DECIMAL x, WM_DECIMAL y, uint32 length)
  {
    return wasmNoise.GetPerlinStrip(x, y, length);
  }
}
