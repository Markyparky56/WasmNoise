#include "WasmNoise.hpp"
#include <stdlib.h> // Included from libc, allows us to import malloc and free

extern "C"
{
  void consolelogi(size_t outI);
}

// We need to override the global new/delete operators to use our own malloc/free functions
// which allocate and free memory in the WebAssembly modules linear memory,
// otherwise they'll be requested as imports
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

  void SetInterp(int32 _interp) { wasmNoise.SetInterp(static_cast<WasmNoise::Interp>(_interp)); }
  int32 GetInterp() { return static_cast<int32>(wasmNoise.GetInterp()); }

  // The non "bulk" or "batch" functions (GetPerlin2, GetPerlin3) 
  // are slower than their counterparts because there is a noticable overhead
  // for calling an exported WebAssembly function from javascript, hence why
  // I've implemented Strip, Square and Cube alternatives which achieve near
  // native speeds and return pointers to their generated values
  WN_INLINE WN_DECIMAL  GetPerlin2(WN_DECIMAL x, WN_DECIMAL y) { return wasmNoise.GetPerlin(x, y); }
  WN_INLINE WN_DECIMAL *GetPerlin2_Strip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length) { return wasmNoise.GetPerlinStrip(startX, startY, length); }
  WN_INLINE WN_DECIMAL *GetPerlin2_Square(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, uint32 height) { return wasmNoise.GetPerlinSquare(startX, startY, length, height); }

  WN_INLINE WN_DECIMAL  GetPerlin3(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) { return wasmNoise.GetPerlin(x, y, z); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Strip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length) { return wasmNoise.GetPerlinStrip(startX, startY, startZ, length); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Square(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height) { return wasmNoise.GetPerlinSquare(startX, startY, startZ, width, height); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Cube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth) { return wasmNoise.GetPerlinCube(startX, startY, startZ, width, height, depth); }
}

