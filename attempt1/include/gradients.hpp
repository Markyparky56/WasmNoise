#pragma once
#include "points.hpp"

namespace Gradient
{
  // Diag constants are precomputed normalised vector element

  template<typename T>
  static Point2<T> get2(uint8_t i)
  {
    static const T diag = static_cast<T>(0.70710678118); 

    switch (i % 8)
    {
    case 0: return {   1.0,   0.0 };
    case 1: return {  -1.0,   0.0 };
    case 2: return {   0.0,   1.0 };
    case 3: return {   0.0,  -1.0 };
    case 4: return {  diag,  diag };
    case 5: return { -diag,  diag };
    case 6: return {  diag, -diag };
    case 7: return { -diag, -diag };
    default: return {-1, -1}; // Throw? Abort?
    }
  }

  template<typename T>
  static Point3<T> get3(uint8_t i)
  {
    static const T diag = static_cast<T>(0.70710678118);
    static const T diag2 = static_cast<T>(0.5773502691896258);

    switch (i % 32)
    {
    case 0:  case 12: return {   diag,  diag,     0.0 };
    case 1:  case 13: return {  -diag,  diag,     0.0 };
    case 2:  case 14: return {   diag, -diag,     0.0 };
    case 3:  case 15: return {  -diag, -diag,     0.0 };
    case 4:  case 16: return {   diag,   0.0,    diag };
    case 5:  case 17: return {  -diag,   0.0,    diag };
    case 6:  case 18: return {   diag,   0.0,   -diag };
    case 7:  case 19: return {  -diag,   0.0,   -diag };
    case 8:  case 20: return {    0.0,  diag,    diag };
    case 9:  case 21: return {    0.0, -diag,    diag };
    case 10: case 22: return {    0.0,  diag,   -diag };
    case 11: case 23: return {    0.0, -diag,   -diag };
    case 24:          return {  diag2,  diag2,  diag2 };
    case 25:          return { -diag2,  diag2,  diag2 };
    case 26:          return {  diag2, -diag2,  diag2 };
    case 27:          return { -diag2, -diag2,  diag2 };
    case 28:          return {  diag2,  diag2, -diag2 };
    case 29:          return { -diag2,  diag2, -diag2 };
    case 30:          return {  diag2, -diag2, -diag2 };
    case 31:          return { -diag2, -diag2, -diag2 };
    default: abort();
    }
  }

  template<typename T>
  static Point4<T> get4(uint8_t i)
  {
    static const T diag = static_cast<T>(0.5773502691896258);
    static const T diag2 = static_cast<T>(0.5);

    switch (i % 64)
    {
      // 32 edges, then 16 corners repeated twice
    case 0:           return {    0.0,   diag,   diag,   diag };
    case 1:           return {    0.0,   diag,   diag,  -diag };
    case 2:           return {    0.0,   diag,  -diag,   diag };
    case 3:           return {    0.0,   diag,  -diag,  -diag };
    case 4:           return {    0.0,  -diag,   diag,   diag };
    case 5:           return {    0.0,  -diag,   diag,  -diag };
    case 6:           return {    0.0,  -diag,  -diag,   diag };
    case 7:           return {    0.0,  -diag,  -diag,  -diag };
    case 8:           return {   diag,    0.0,   diag,   diag };
    case 9:           return {   diag,    0.0,   diag,  -diag };
    case 10:          return {   diag,    0.0,  -diag,   diag };
    case 11:          return {   diag,    0.0,  -diag,  -diag };
    case 12:          return {  -diag,    0.0,   diag,   diag };
    case 13:          return {  -diag,    0.0,   diag,  -diag };
    case 14:          return {  -diag,    0.0,  -diag,   diag };
    case 15:          return {  -diag,    0.0,  -diag,  -diag };
    case 16:          return {   diag,   diag,    0.0,   diag };
    case 17:          return {   diag,   diag,    0.0,  -diag };
    case 18:          return {   diag,  -diag,    0.0,   diag };
    case 19:          return {   diag,  -diag,    0.0,  -diag };
    case 20:          return {  -diag,   diag,    0.0,   diag };
    case 21:          return {  -diag,   diag,    0.0,  -diag };
    case 22:          return {  -diag,  -diag,    0.0,   diag };
    case 23:          return {  -diag,  -diag,    0.0,  -diag };
    case 24:          return {   diag,   diag,   diag,    0.0 };
    case 25:          return {   diag,   diag,  -diag,    0.0 };
    case 26:          return {   diag,  -diag,   diag,    0.0 };
    case 27:          return {   diag,  -diag,  -diag,    0.0 };
    case 28:          return {  -diag,   diag,   diag,    0.0 };
    case 29:          return {  -diag,   diag,  -diag,    0.0 };
    case 30:          return {  -diag,  -diag,   diag,    0.0 };
    case 31:          return {  -diag,  -diag,  -diag,    0.0 };
    case 32: case 48: return {  diag2,  diag2,  diag2,  diag2 };
    case 33: case 49: return { -diag2,  diag2,  diag2,  diag2 };
    case 34: case 50: return {  diag2, -diag2,  diag2,  diag2 };
    case 35: case 51: return { -diag2, -diag2,  diag2,  diag2 };
    case 36: case 52: return {  diag2,  diag2, -diag2,  diag2 };
    case 37: case 53: return { -diag2,  diag2, -diag2,  diag2 };
    case 38: case 54: return {  diag2,  diag2,  diag2, -diag2 };
    case 39: case 55: return { -diag2,  diag2,  diag2, -diag2 };
    case 40: case 56: return {  diag2, -diag2, -diag2,  diag2 };
    case 41: case 57: return { -diag2, -diag2, -diag2,  diag2 };
    case 42: case 58: return {  diag2, -diag2,  diag2, -diag2 };
    case 43: case 59: return { -diag2, -diag2,  diag2, -diag2 };
    case 44: case 60: return {  diag2,  diag2, -diag2, -diag2 };
    case 45: case 61: return { -diag2,  diag2, -diag2, -diag2 };
    case 46: case 62: return {  diag2, -diag2, -diag2, -diag2 };
    case 47: case 63: return { -diag2, -diag2, -diag2, -diag2 };
    default: abort();
    }
  }
}
