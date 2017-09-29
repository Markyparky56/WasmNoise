#pragma once
#include "point2.hpp"
#include "point3.hpp"
#include "point4.hpp"

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#define PM_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define PM_INLINE __forceinline 
#else
#define PM_INLINE inline
#endif

namespace Points
{
  using int2 = Point2<int>;
  using int3 = Point3<int>;
  using int4 = Point4<int>;

  using float2 = Point2<float>;
  using float3 = Point3<float>;
  using float4 = Point4<float>;

  using double2 = Point2<double>;
  using double3 = Point3<double>;
  using double4 = Point4<double>;
}