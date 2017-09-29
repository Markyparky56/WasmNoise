#pragma once
#include "pointmath.hpp"
#include "permutationtable.hpp"
#include "gradients.hpp"
#include <cmath>
#include <emscripten/bind.h>

namespace Perlin
{
  class PerlinGenerator
  {
  private:
    PermutationTable permutationTable;
    uint32_t seed;

    // Surflet functions
    template<typename T>
    PM_INLINE T surflet2(PermutationTable &permTable, Point2<int8_t> &corner, Point2<T> &distance)
    {
      T attn = static_cast<T>(1.0) - PointMath::dot2(distance, distance);
      if (attn > static_cast<T>(0.0))
      {
        auto grad = Gradient::get2<T>(permTable.get2(corner));
        return (attn*attn*attn*attn) * PointMath::dot2(distance, grad);
      }
      else
      {
        return static_cast<T>(0.0);
      }
    }

    template<typename T>
    PM_INLINE T surflet3(PermutationTable &permTable, Point3<int8_t> &corner, Point3<T> &distance)
    {
      T attn = static_cast<T>(1.0) - PointMath::dot3(distance, distance);
      if (attn > static_cast<T>(0.0))
      {
        auto grad = Gradient::get3<T>(permTable.get3(corner));
        return (attn*attn*attn*attn) * PointMath::dot3(distance, grad);
      }
      else
      {
        return static_cast<T>(0.0);
      }
    }

    template<typename T>
    PM_INLINE T surflet4(PermutationTable &permTable, Point4<int8_t> &corner, Point4<T> &distance)
    {
      T attn = static_cast<T>(1.0) - PointMath::dot4(distance, distance);
      if (attn > static_cast<T>(0.0))
      {
        auto grad = Gradient::get4<T>(permTable.get4(corner));
        return (attn*attn*attn*attn) * PointMath::dot4(distance, grad);
      }
      else
      {
        return static_cast<T>(0.0);
      }
    }
  public:
    PerlinGenerator()
      : permutationTable(PermutationTable()) // Classic permuation table
      , seed(0)
    {}
    PerlinGenerator(uint32_t _seed)
      : permutationTable(PermutationTable::NewTable(_seed))
      , seed(_seed)
    {}      

    void SetSeed(uint32_t newSeed)
    {
      permutationTable = PermutationTable::NewTable(newSeed);
      seed = newSeed;
    }

    // Evaluation functions
    template<typename T> T eval(Point2<T> &p);
    template<typename T> T eval(Point3<T> &p);
    template<typename T> T eval(Point4<T> &p);
    template<typename T> PM_INLINE T eval(T x, T y)           
    { 
      auto&& val = Point2<T>{ x, y };
      return eval(val); 
    }
    template<typename T> PM_INLINE T eval(T x, T y, T z)      
    {
      auto && val = Point3<T>{ x, y, z };
      return eval(val); 
    }
    template<typename T> PM_INLINE T eval(T x, T y, T z, T w) 
    { 
      auto&& val = Point4<T>{ x, y, z, w };
      return eval(val); 
    }

  };

  // 2D Noise Function
  template<typename T>
  T Perlin::PerlinGenerator::eval(Point2<T> &p)
  {
    Point2<T> floored = PointMath::map2(p, PointMath::MapOps::FastFloor);
    Point2<int8_t> nearCorner = PointMath::convert2<T, int8_t>(floored).result;
    Point2<int8_t> farCorner = PointMath::add1_2(nearCorner);
    Point2<T> nearDistance = PointMath::sub2(p, floored);
    Point2<T> farDistance = PointMath::sub1_2(nearDistance);

    auto&& pf10_i = Point2<int8_t>{farCorner[0], nearCorner[1]};
    auto&& pf10_T = Point2<T>{farDistance[0], nearDistance[1]};
    auto&& pf01_i = Point2<int8_t>{nearCorner[0], farCorner[1]};
    auto&& pf01_T = Point2<T>{nearDistance[0], farDistance[1]};

    T f00 = surflet2(permutationTable, nearCorner, nearDistance);
    T f10 = surflet2(permutationTable, pf10_i, pf10_T);
    T f01 = surflet2(permutationTable, pf01_i, pf01_T);
    T f11 = surflet2(permutationTable, farCorner, farDistance);

    return (f00 + f10 + f01 + f11) * static_cast<T>(3.1604938271604937);
  }

  // 3D Noise function
  template<typename T>
  T Perlin::PerlinGenerator::eval(Point3<T> &p)
  {
    Point3<T> floored = PointMath::map3(p, PointMath::MapOps::FastFloor);
    Point3<int8_t> nearCorner = PointMath::convert3<T, int8_t>(floored).result;
    Point3<int8_t> farCorner = PointMath::add1_3(nearCorner);
    Point3<T> nearDistance = PointMath::sub3(p, floored);
    Point3<T> farDistance = PointMath::sub1_3(nearDistance);

    auto&& pf100_i = Point3<int8_t>{farCorner[0], nearCorner[1], nearCorner[2]};
    auto&& pf100_T = Point3<T>{farDistance[0], nearDistance[1], nearDistance[2]};
    auto&& pf010_i = Point3<int8_t>{nearCorner[0], farCorner[1], nearCorner[2]};
    auto&& pf010_T = Point3<T>{nearDistance[0], farDistance[1], nearDistance[2]};
    auto&& pf110_i = Point3<int8_t>{farCorner[0], farCorner[1], nearCorner[2]};
    auto&& pf110_T = Point3<T>{farDistance[0], farDistance[1], nearDistance[2]};
    auto&& pf001_i = Point3<int8_t>{nearCorner[0], nearCorner[1], farCorner[2]};
    auto&& pf001_T = Point3<T>{nearDistance[0], nearDistance[1], farDistance[2]};
    auto&& pf101_i = Point3<int8_t>{farCorner[0], nearCorner[1], farCorner[2]};
    auto&& pf101_T = Point3<T>{farDistance[0], nearDistance[1], farDistance[2]};
    auto&& pf011_i = Point3<int8_t>{nearCorner[0], farCorner[1], farCorner[2]};
    auto&& pf011_T = Point3<T>{nearDistance[0], farDistance[1], farDistance[2]};

    T f000 = surflet3( permutationTable
                     , nearCorner
                     , nearDistance);
    T f100 = surflet3( permutationTable
                     , pf100_i
                     , pf100_T);
    T f010 = surflet3( permutationTable
                     , pf010_i
                     , pf010_T);
    T f110 = surflet3( permutationTable
                     , pf110_i
                     , pf110_T);
    T f001 = surflet3( permutationTable
                     , pf001_i
                     , pf001_T);
    T f101 = surflet3( permutationTable
                     , pf101_i
                     , pf101_T);
    T f011 = surflet3( permutationTable
                     , pf011_i
                     , pf011_T);
    T f111 = surflet3( permutationTable
                     , farCorner
                     , farDistance);

    return (f000 + f100 + f010 
          + f110 + f001 + f101 
          + f011 + f111) * static_cast<T>(3.8898553255531074);
  }

  // 4D Noise function
  template<typename T>
  T Perlin::PerlinGenerator::eval(Point4<T> &p)
  {
    Point4<T> floored = PointMath::map4(p, PointMath::MapOps::FastFloor);
    Point4<int8_t> nearCorner = PointMath::convert4<T, int8_t>(floored).result;
    Point4<int8_t> farCorner = PointMath::add1_4(nearCorner);
    Point4<T> nearDistance = PointMath::sub4(p, floored);
    Point4<T> farDistance = PointMath::sub1_4(nearDistance);

    auto&& pf1000_i = Point4<int8_t>{farCorner[0], nearCorner[1], nearCorner[2], nearCorner[3]};
    auto&& pf1000_T = Point4<T>{farDistance[0], nearDistance[1], nearDistance[2], nearDistance[3]};
    auto&& pf0100_i = Point4<int8_t>{nearCorner[0], farCorner[1], nearCorner[2], nearCorner[3]};
    auto&& pf0100_T = Point4<T>{nearDistance[0], farDistance[1], nearDistance[2], nearDistance[3]};
    auto&& pf1100_i = Point4<int8_t>{farCorner[0], farCorner[1], nearCorner[2], nearCorner[3]};
    auto&& pf1100_T = Point4<T>{farDistance[0], farDistance[1], nearDistance[2], nearDistance[3]};
    auto&& pf0010_i = Point4<int8_t>{nearCorner[0], nearCorner[1], farCorner[2], nearCorner[3]};
    auto&& pf0010_T = Point4<T>{nearDistance[0], nearDistance[1], farDistance[2], nearDistance[3]};
    auto&& pf1010_i = Point4<int8_t>{farCorner[0], nearCorner[1], farCorner[2], nearCorner[3]};
    auto&& pf1010_T = Point4<T>{farDistance[0], nearDistance[1], farDistance[2], nearDistance[3]};
    auto&& pf0110_i = Point4<int8_t>{nearCorner[0], farCorner[1], farCorner[2], nearCorner[3]};
    auto&& pf0110_T = Point4<T>{nearDistance[0], farDistance[1], farDistance[2], nearDistance[3]};
    auto&& pf1110_i = Point4<int8_t>{farCorner[0], farCorner[1], farCorner[2], nearCorner[3]};
    auto&& pf1110_T = Point4<T>{farDistance[0], farDistance[1], farDistance[2], nearDistance[3]};
    auto&& pf0001_i = Point4<int8_t>{nearCorner[0], nearCorner[1], nearCorner[2], farCorner[3]};
    auto&& pf0001_T = Point4<T>{nearDistance[0], nearDistance[1], nearDistance[2], farDistance[3]};
    auto&& pf1001_i = Point4<int8_t>{farCorner[0], nearCorner[1], nearCorner[2], farCorner[3]};
    auto&& pf1001_T = Point4<T>{farDistance[0], nearDistance[1], nearDistance[2], farDistance[3]};
    auto&& pf0101_i = Point4<int8_t>{nearCorner[0], farCorner[1], nearCorner[2], farCorner[3]};
    auto&& pf0101_T = Point4<T>{nearDistance[0], farDistance[1], nearDistance[2], farDistance[3]};
    auto&& pf1101_i = Point4<int8_t>{farCorner[0], farCorner[1], nearCorner[2], farCorner[3]};
    auto&& pf1101_T = Point4<T>{farDistance[0], farDistance[1], nearDistance[2], farDistance[3]};
    auto&& pf0011_i = Point4<int8_t>{nearCorner[0], nearCorner[1], farCorner[2], farCorner[3]};
    auto&& pf0011_T = Point4<T>{nearDistance[0], nearDistance[1], farDistance[2], farDistance[3]};
    auto&& pf1011_i = Point4<int8_t>{farCorner[0], nearCorner[1], farCorner[2], farCorner[3]};
    auto&& pf1011_T = Point4<T>{farDistance[0], nearDistance[1], farDistance[2], farDistance[3]};
    auto&& pf0111_i = Point4<int8_t>{nearCorner[0], farCorner[1], farCorner[2], farCorner[3]};
    auto&& pf0111_T = Point4<T>{nearDistance[0], farDistance[1], farDistance[2], farDistance[3]};

    T f0000 = surflet4( permutationTable
                      , nearCorner
                      , nearDistance);
    T f1000 = surflet4( permutationTable
                      , pf1000_i
                      , pf1000_T);
    T f0100 = surflet4( permutationTable
                      , pf0100_i
                      , pf0100_T);
    T f1100 = surflet4( permutationTable
                      , pf1100_i
                      , pf1100_T);
    T f0010 = surflet4( permutationTable
                      , pf0010_i
                      , pf0010_T);
    T f1010 = surflet4( permutationTable
                      , pf1010_i
                      , pf1010_T);
    T f0110 = surflet4( permutationTable
                      , pf0110_i
                      , pf0110_T);
    T f1110 = surflet4( permutationTable
                      , pf1110_i
                      , pf1110_T);
    T f0001 = surflet4( permutationTable
                      , pf0001_i
                      , pf0001_T);
    T f1001 = surflet4( permutationTable
                      , pf1001_i
                      , pf1001_T);
    T f0101 = surflet4( permutationTable
                      , pf0101_i
                      , pf0101_T);
    T f1101 = surflet4( permutationTable
                      , pf1101_i
                      , pf1101_T);
    T f0011 = surflet4( permutationTable
                      , pf0011_i
                      , pf0011_T);
    T f1011 = surflet4( permutationTable
                      , pf1011_i
                      , pf1011_T);
    T f0111 = surflet4( permutationTable
                      , pf0111_i
                      , pf0111_T);
    T f1111 = surflet4( permutationTable
                      , farCorner
                      , farDistance);

    return (f0000 + f1000 + f0100 + f1100 
          + f0010 + f1010 + f0110 + f1110 
          + f0001 + f1001 + f0101 + f1101 
          + f0011 + f1011 + f0111 + f1111) * static_cast<T>(4.424369240215691);
  }
}

EMSCRIPTEN_BINDINGS(Perlin)
{
  emscripten::class_<Perlin::PerlinGenerator>("PerlinGenerator")
    .constructor<>()
    .constructor<uint32_t>()
    .function("SetSeed", &Perlin::PerlinGenerator::SetSeed)
    .function("evalXY", emscripten::select_overload<double(double, double)>(&Perlin::PerlinGenerator::eval<double>))
    .function("evalXYZ", emscripten::select_overload<double(double, double, double)>(&Perlin::PerlinGenerator::eval<double>))
    .function("evalXYZW", emscripten::select_overload<double(double, double, double, double)>(&Perlin::PerlinGenerator::eval<double>))
    /*.function("evalPXY", emscripten::select_overload<double(Point2<double>&)>(&Perlin::PerlinGenerator::eval<double>))
    .function("evalPXYZ", emscripten::select_overload<double(Point3<double>&)>(&Perlin::PerlinGenerator::eval<double>))
    .function("evalPXYZW", emscripten::select_overload<double(Point4<double>&)>(&Perlin::PerlinGenerator::eval<double>))*/;
}
