#pragma once
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#define WN_INLINE __attribute__((always_inline))
#define NO_ALIAS __attribute__((__malloc__))
#elif defined(_MSC_VER)
#define WN_INLINE __forceinline 
#define NO_ALIAS 
#else
#define WN_INLINE inline
#define NO_ALIAS 
#endif

#ifdef WN_USE_DOUBLES
using WN_DECIMAL = double;
#else
using WN_DECIMAL = float;
#endif
