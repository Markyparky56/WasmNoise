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

#ifdef WN_INCLUDE_FRACTAL_GETSET
  void SetFractalOctaves(uint32 _octaves) { wasmNoise.SetFractalOctaves(_octaves); }
  uint32 GetFractalOctaves() { return wasmNoise.GetFractalOctaves(); }

  void SetFractalLacunarity(WN_DECIMAL _lacunarity) { wasmNoise.SetFractalLacunarity(_lacunarity); }
  WN_DECIMAL GetFractalLacunarity() { return wasmNoise.GetFractalLacunarity(); }

  void SetFractalGain(WN_DECIMAL _gain) { wasmNoise.SetFractalGain(_gain); }
  WN_DECIMAL GetFractalGain() { return wasmNoise.GetFractalGain(); }

  void SetFractalType(int32 _fractalType) { wasmNoise.SetFractalType(static_cast<WasmNoise::FractalType>(_fractalType)); }
  int32 GetFractalType() { return static_cast<int32>(wasmNoise.GetFractalType()); }
#endif // WN_INCLUDE_FRACTAL_GETSET

  // The non "bulk" or "batch" functions (GetPerlin2, GetPerlin3 etc.) 
  // are slower than their counterparts because there is a noticable overhead
  // for calling an exported WebAssembly function from javascript, hence why
  // I've implemented Strip, Square and Cube alternatives which achieve near
  // native speeds and return pointers to their generated values
#ifdef WN_INCLUDE_PERLIN
  WN_INLINE WN_DECIMAL  GetPerlin2(WN_DECIMAL x, WN_DECIMAL y) { return wasmNoise.GetPerlin(x, y); }
  WN_INLINE WN_DECIMAL *GetPerlin2_Strip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, int32 direction) { return wasmNoise.GetPerlinStrip(startX, startY, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetPerlin2_Square(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, uint32 height) { return wasmNoise.GetPerlinSquare(startX, startY, length, height); }

  WN_INLINE WN_DECIMAL  GetPerlin3(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) { return wasmNoise.GetPerlin(x, y, z); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Strip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, int32 direction) { return wasmNoise.GetPerlinStrip(startX, startY, startZ, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Square(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, int32 plane) { return wasmNoise.GetPerlinSquare(startX, startY, startZ, width, height, static_cast<WasmNoise::SquarePlane>(plane)); }
  WN_INLINE WN_DECIMAL *GetPerlin3_Cube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth) { return wasmNoise.GetPerlinCube(startX, startY, startZ, width, height, depth); }
#endif // WN_INCLUDE_PERLIN
#ifdef WN_INCLUDE_PERLIN_FRACTAL
  WN_INLINE WN_DECIMAL  GetPerlinFractal2(WN_DECIMAL x, WN_DECIMAL y) { return wasmNoise.GetPerlinFractal(x, y); }
  WN_INLINE WN_DECIMAL *GetPerlinFractal2_Strip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, int32 direction) { return wasmNoise.GetPerlinFractalStrip(startX, startY, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetPerlinFractal2_Square(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, uint32 height) { return wasmNoise.GetPerlinFractalSquare(startX, startY, length, height); }

  WN_INLINE WN_DECIMAL  GetPerlinFractal3(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) { return wasmNoise.GetPerlinFractal(x, y, z); }
  WN_INLINE WN_DECIMAL *GetPerlinFractal3_Strip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, int32 direction) { return wasmNoise.GetPerlinFractalStrip(startX, startY, startZ, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetPerlinFractal3_Square(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, int32 plane) { return wasmNoise.GetPerlinFractalSquare(startX, startY, startZ, width, height, static_cast<WasmNoise::SquarePlane>(plane)); }
  WN_INLINE WN_DECIMAL *GetPerlinFractal3_Cube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth) { return wasmNoise.GetPerlinFractalCube(startX, startY, startZ, width, height, depth); }
#endif // WN_INCLUDE_PERLIN_FRACTAL
#ifdef WN_INCLUDE_SIMPLEX
  WN_INLINE WN_DECIMAL  GetSimplex2(WN_DECIMAL x, WN_DECIMAL y) { return wasmNoise.GetSimplex(x, y); }
  WN_INLINE WN_DECIMAL *GetSimplex2_Strip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, int32 direction) { return wasmNoise.GetSimplexStrip(startX, startY, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetSimplex2_Square(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height) { return wasmNoise.GetSimplexSquare(startX, startY, width, height); }

  WN_INLINE WN_DECIMAL  GetSimplex3(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) { return wasmNoise.GetSimplex(x, y, z); }
  WN_INLINE WN_DECIMAL *GetSimplex3_Strip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, int32 direction) { return wasmNoise.GetSimplexStrip(startX, startY, startZ, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetSimplex3_Square(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, int32 plane) { return wasmNoise.GetSimplexSquare(startX, startY, startZ, width, height, static_cast<WasmNoise::SquarePlane>(plane)); }
  WN_INLINE WN_DECIMAL *GetSimplex3_Cube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth) { return wasmNoise.GetSimplexCube(startX, startY, startZ, width, height, depth); }

  WN_INLINE WN_DECIMAL  GetSimplex4(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w) { return wasmNoise.GetSimplex(x, y, z, w); }
  WN_INLINE WN_DECIMAL *GetSimplex4_Strip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 length, int32 direction) { return wasmNoise.GetSimplexStrip(startX, startY, startZ, startW, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetSimplex4_Square(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, int32 plane) { return wasmNoise.GetSimplexSquare(startX, startY, startZ, startW, width, height, static_cast<WasmNoise::SquarePlane>(plane)); }
  WN_INLINE WN_DECIMAL *GetSimplex4_Cube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, uint32 depth) { return wasmNoise.GetSimplexCube(startX, startY, startZ, startW, width, height, depth); }
#endif // WN_INCLUDE_SIMPLEX
#ifdef WN_INCLUDE_SIMPLEX_FRACTAL
  WN_INLINE WN_DECIMAL  GetSimplexFractal2(WN_DECIMAL x, WN_DECIMAL y) { return wasmNoise.GetSimplexFractal(x, y); }
  WN_INLINE WN_DECIMAL *GetSimplexFractal2_Strip(WN_DECIMAL startX, WN_DECIMAL startY, uint32 length, int32 direction) { return wasmNoise.GetSimplexFractalStrip(startX, startY, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetSimplexFractal2_Square(WN_DECIMAL startX, WN_DECIMAL startY, uint32 width, uint32 height) { return wasmNoise.GetSimplexFractalSquare(startX, startY, width, height); }

  WN_INLINE WN_DECIMAL  GetSimplexFractal3(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z) { return wasmNoise.GetSimplexFractal(x, y, z); }
  WN_INLINE WN_DECIMAL *GetSimplexFractal3_Strip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 length, int32 direction) { return wasmNoise.GetSimplexFractalStrip(startX, startY, startZ, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetSimplexFractal3_Square(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, int32 plane) { return wasmNoise.GetSimplexFractalSquare(startX, startY, startZ, width, height, static_cast<WasmNoise::SquarePlane>(plane)); }
  WN_INLINE WN_DECIMAL *GetSimplexFractal3_Cube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, uint32 width, uint32 height, uint32 depth) { return wasmNoise.GetSimplexFractalCube(startX, startY, startZ, width, height, depth); }

  WN_INLINE WN_DECIMAL  GetSimplexFractal4(WN_DECIMAL x, WN_DECIMAL y, WN_DECIMAL z, WN_DECIMAL w) { return wasmNoise.GetSimplexFractal(x, y, z, w); }
  WN_INLINE WN_DECIMAL *GetSimplexFractal4_Strip(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 length, int32 direction) { return wasmNoise.GetSimplexFractalStrip(startX, startY, startZ, startW, length, static_cast<WasmNoise::StripDirection>(direction)); }
  WN_INLINE WN_DECIMAL *GetSimplexFractal4_Square(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, int32 plane) { return wasmNoise.GetSimplexFractalSquare(startX, startY, startZ, startW, width, height, static_cast<WasmNoise::SquarePlane>(plane)); }
  WN_INLINE WN_DECIMAL *GetSimplexFractal4_Cube(WN_DECIMAL startX, WN_DECIMAL startY, WN_DECIMAL startZ, WN_DECIMAL startW, uint32 width, uint32 height, uint32 depth) { return wasmNoise.GetSimplexFractalCube(startX, startY, startZ, startW, width, height, depth); }
#endif // WN_INCLUDE_SIMPLEX_FRACTAL
}

