#pragma once
#include "points.hpp"

namespace PointMath
{
  // Add
  template<typename T>
  PM_INLINE Point2<T> add2(Point2<T> &a, Point2<T> &b)
  {
    return Point2<T>(a[0] + b[0], a[1] + b[1]);
  }

  template<typename T>
  PM_INLINE Point3<T> add3(Point3<T> &a, Point3<T> &b)
  {
    return Point3<T>(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
  }

  template<typename T>
  PM_INLINE Point4<T> add4(Point4<T> &a, Point4<T> &b)
  {
    return Point4<T>(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
  }

  // Subtract
  template<typename T>
  PM_INLINE Point2<T> sub2(Point2<T> &a, Point2<T> &b)
  {
    return Point2<T>(a[0] - b[0], a[1] - b[1]);
  }

  template<typename T>
  PM_INLINE Point3<T> sub3(Point3<T> &a, Point3<T> &b)
  {
    return Point3<T>(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
  }

  template<typename T>
  PM_INLINE Point4<T> sub4(Point4<T> &a, Point4<T> &b)
  {
    return Point4<T>(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
  }

  // Divide
  template<typename T>
  PM_INLINE Point2<T> div2(Point2<T> &a, Point2<T> &b)
  {
    return Point2<T>(a[0] / b[0], a[1] / b[1]);
  }

  template<typename T>
  PM_INLINE Point3<T> div3(Point3<T> &a, Point3<T> &b)
  {
    return Point3<T>(a[0] / b[0], a[1] / b[1], a[2] / b[2]);
  }

  template<typename T>
  PM_INLINE Point4<T> div4(Point4<T> &a, Point4<T> &b)
  {
    return Point4<T>(a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]);
  }

  // Multiply
  template<typename T>
  PM_INLINE Point2<T> mul2(Point2<T> &a, Point2<T> &b)
  {
    return Point2<T>(a[0] * b[0], a[1] * b[1]);
  }

  template<typename T>
  PM_INLINE Point3<T> mul3(Point3<T> &a, Point3<T> &b)
  {
    return Point3<T>(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
  }

  template<typename T>
  PM_INLINE Point4<T> mul4(Point4<T> &a, Point4<T> &b)
  {
    return Point4<T>(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
  }  

  // Fold (Perform an operation on each item reducing down to a single value)
  // Fold operations
  namespace FoldOp
  {
    template<typename T> using OpPtr = T(*)(T, T);

    template<typename T> PM_INLINE T AddOp(T a, T b) { return a + b; }
    template<typename T> PM_INLINE T SubOp(T a, T b) { return a - b; }
    template<typename T> PM_INLINE T DivOp(T a, T b) { return a / b; }
    template<typename T> PM_INLINE T MulOp(T a, T b) { return a * b; }
  }

  template<typename T> PM_INLINE T fold2(Point2<T> &a, FoldOp::OpPtr<T> op) { return op(a[0], a[1]); };
  template<typename T> PM_INLINE T fold3(Point3<T> &a, FoldOp::OpPtr<T> op) { return op(op(a[0], a[1]), a[2]); }
  template<typename T> PM_INLINE T fold4(Point4<T> &a, FoldOp::OpPtr<T> op) { return op(op(op(a[0], a[1]), a[2]), a[3]); }

  // We can leverage the fold functions to perform a dot product
  template<typename T> PM_INLINE T dot2(Point2<T> &a, Point2<T> &b) { return fold2(mul2(a, b), FoldOp::AddOp); }
  template<typename T> PM_INLINE T dot3(Point3<T> &a, Point3<T> &b) { return fold3(mul3(a, b), FoldOp::AddOp); }
  template<typename T> PM_INLINE T dot4(Point4<T> &a, Point4<T> &b) { return fold4(mul4(a, b), FoldOp::AddOp); }

  // More complex mathematical functions for performing on point objects
  namespace MapOps
  {
    template<typename T> using Point2OpPtr = Point2<T>(*)(Point2<T> &);
    template<typename T> using Point3OpPtr = Point3<T>(*)(Point3<T> &);
    template<typename T> using Point4OpPtr = Point4<T>(*)(Point4<T> &);

    // Fast Floor and Ceiling functions are based on those at
    // https://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions
    // The shifting amount may cause complications/inaccurracies and errors on large values
    // But should suffice for smaller values
    template<typename T> PM_INLINE Point2<T> FastFloor(Point2<T> &p)
    {
      return
      {
        static_cast<T>(static_cast<int>(p[0] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[1] + 65536.0) - 65536)
      };
    }
    
    template<typename T> PM_INLINE Point3<T> FastFloor(Point3<T> &p)
    {
      return
      {
        static_cast<T>(static_cast<int>(p[0] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[1] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[2] + 65536.0) - 65536)
      };
    }
    template<typename T> PM_INLINE Point4<T> FastFloor(Point4<T> &p)
    {
      return
      {
        static_cast<T>(static_cast<int>(p[0] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[1] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[2] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[3] + 65536.0) - 65536)
      };
    }

    template<typename T> PM_INLINE Point2<T> FastCeiling(Point2<T> &p)
    {
      return
      {
        static_cast<T>(static_cast<int>(p[0] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[1] + 65536.0) - 65536)
      };
    }
    template<typename T> PM_INLINE Point3<T> FastCeiling(Point3<T> &p)
    {
      return
      {
        static_cast<T>(static_cast<int>(p[0] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[1] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[2] + 65536.0) - 65536)
      };
    }
    template<typename T> PM_INLINE Point4<T> FastCeiling(Point4<T> &p)
    {
      return
      {
        static_cast<T>(static_cast<int>(p[0] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[1] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[2] + 65536.0) - 65536),
        static_cast<T>(static_cast<int>(p[3] + 65536.0) - 65536)
      };
    }
  }

  template<typename T> PM_INLINE Point2<T> map2(Point2<T> &a, MapOps::Point2OpPtr<T> op) { return op(a); }
  template<typename T> PM_INLINE Point3<T> map3(Point3<T> &a, MapOps::Point3OpPtr<T> op) { return op(a); }
  template<typename T> PM_INLINE Point4<T> map4(Point4<T> &a, MapOps::Point4OpPtr<T> op) { return op(a); }

  // Conversion functions for converting a point from one type to another (such as float to int, or int to float)
  // The old and new types are required template parameters when calling, 
  // and you must retrieve the result from the object returned
  template<template<typename T> class NewType, typename T>
  class ConvertResult
  {
  public:
    NewType<T> result;
  };
  
  template<typename OldType, typename NewType> PM_INLINE ConvertResult< Point2, NewType > convert2(Point2<OldType> &p)
  {
    return ConvertResult<Point2, NewType>
    {
      {
        static_cast<NewType>(p[0]),
        static_cast<NewType>(p[1])
      }
    };
  }

  template<typename OldType, typename NewType> PM_INLINE ConvertResult< Point3, NewType > convert3(Point3<OldType> &p)
  {
    return ConvertResult<Point3, NewType>
    {
      {
        static_cast<NewType>(p[0]),
        static_cast<NewType>(p[1]),
        static_cast<NewType>(p[2])
      }
    };
  }

  template<typename OldType, typename NewType> PM_INLINE ConvertResult< Point4, NewType > convert4(Point4<OldType> &p)
  {
    return ConvertResult<Point4, NewType>
    {
      {
        static_cast<NewType>(p[0]),
        static_cast<NewType>(p[1]),
        static_cast<NewType>(p[2]),
        static_cast<NewType>(p[3])
      }
    };
  }

  // Helper functions for getting a Point with all 1s
  template<typename T> PM_INLINE Point2<T> one2()
  {
    return Point2<T>
    {
      static_cast<T>(1),
      static_cast<T>(1)
    };
  }
  template<typename T> PM_INLINE Point3<T> one3()
  {
    return Point3<T>
    {
      static_cast<T>(1),
      static_cast<T>(1),
      static_cast<T>(1)
    };
  }
  template<typename T> PM_INLINE Point4<T> one4()
  {
    return Point4<T>
    {
      static_cast<T>(1),
      static_cast<T>(1),
      static_cast<T>(1),
      static_cast<T>(1)
    };
  }

};
