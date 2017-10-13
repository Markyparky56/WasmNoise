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

  void SetFrequency(WN_DECIMAL _frequency) { wasmNoise.SetFrequency(_frequency); }
  WN_DECIMAL GetFrequency() { return wasmNoise.GetFrequency(); }

  // The non "bulk" or "batch" functions (GetPerlin2, GetPerlin3) 
  // are slower than their counterparts because there is a noticable overhead
  // for calling an exported WebAssembly function from javascript, hence why
  // I've implemented Strip, Square and Cube alternatives which achieve near
  // native speeds and return pointers to their generated values
  WN_INLINE WN_DECIMAL  GetPerlin2(WN_DECIMAL x, WN_DECIMAL y) { return wasmNoise.GetPerlin(x, y); }
  WN_INLINE WN_DECIMAL *GetPerlin2_Strip(WN_DECIMAL x, WN_DECIMAL y, uint32 length) { return wasmNoise.GetPerlinStrip(x, y, length); }
  WN_INLINE WN_DECIMAL *GetPerlin2_Square(WN_DECIMAL x, WN_DECIMAL y, uint32 length, uint32 height) { return wasmNoise.GetPerlinSquare(x, y, length, height); }

  WN_INLINE WN_DECIMAL  GetPerlin3(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) { return wasmNoise.GetPerlin(x, y, z); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Strip(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, uint32 length) { return wasmNoise.GetPerlinStrip(x, y, z, length); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Square(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, uint32 width, uint32 height) { return wasmNoise.GetPerlinSquare(x, y, z, width, height); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Cube(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, uint32 width, uint32 height, uint32 depth) { return wasmNoise.GetPerlinCube(x, y, z, width, height, depth); }
}

