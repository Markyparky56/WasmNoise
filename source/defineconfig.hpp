#pragma once
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#define WN_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define WN_INLINE __forceinline 
#else
#define WN_INLINE inline
#endif

// Having these defined while editing helps keep the Intelli-sense happy, comment
// them out when building otherwise the compiler will complain about re-definitions
// #define WN_INCLUDE_FRACTAL_GETSET
// #define WN_INCLUDE_CELLULAR_GETSET
// #define WN_INCLUDE_PERLIN 
// #define WN_INCLUDE_PERLIN_FRACTAL
// #define WN_INCLUDE_SIMPLEX
// #define WN_INCLUDE_SIMPLEX_FRACTAL
// #define WN_INCLUDE_CELLULAR
// #define WN_INCLUDE_CELLULAR_FRACTAL

// Until JavaScript and WebAssembly support returning doubles (f64) we're stuck with floats
#ifdef WN_USE_DOUBLES
using WN_DECIMAL = double;
#else
using WN_DECIMAL = float;
#endif

#ifdef WN_ALLOW_ABORT
extern "C"
{
  extern void abort();
  #define ABORT(dummy) abort()   
}
#else
#define ABORT(dummy)
#endif

#define EXPORT __attribute__((visibility ("default")))
#define IMPORT __attribute__((visibility ("default")))
