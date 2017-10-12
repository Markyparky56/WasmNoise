#pragma once
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#define WM_INLINE __attribute__((always_inline))
#define NO_ALIAS __attribute__((__malloc__))
#elif defined(_MSC_VER)
#define WM_INLINE __forceinline 
#define NO_ALIAS 
#else
#define WM_INLINE inline
#define NO_ALIAS 
#endif

#ifdef WM_USE_DOUBLES
using WM_DECIMAL = double;
#else
using WM_DECIMAL = float;
#endif
