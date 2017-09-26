#pragma once
#include "pointmath.hpp"
#include "permutationtable.hpp"
#include "gradients.hpp"
#include <cmath>

namespace Perlin
{
  class PerlinGenerator
  {
  private:
    PermutationTable permutationTable;
    uint64_t seed;

    // Surflet functions
    template<typename T>
    PM_INLINE T surflet2(PermutationTable &permTable, Point2<int8_t> &corner, Point2<T> &distance)
    {
      T attn = static_cast<T>(1.0) - PointMath::dot2(distance, distance);
      if (attn > static_cast<T>(0.0))
      {
        return (attn*attn*attn*attn) * PointMath::dot2(distance, Gradient::get2<T>(permTable.get2(corner)));
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
        return (attn*attn*attn*attn) * PointMath::dot3(distance, Gradient::get3<T>(permTable.get3(corner)));
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
        return (attn*attn*attn*attn) * PointMath::dot4(distance, Gradient::get4<T>(permTable.get4(corner)));
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
    PerlinGenerator(uint64_t _seed)
      : permutationTable(PermutationTable::NewTable(_seed))
      , seed(_seed)
    {}

    void SetSeed(uint64_t newSeed)
    {
      permutationTable = PermutationTable::NewTable(newSeed);
      seed = newSeed;
    }

    // Evaluation functions
    template<typename T> T eval(Point2<T> &p);
    template<typename T> T eval(Point3<T> &p);
    template<typename T> T eval(Point4<T> &p);
    template<typename T> PM_INLINE T eval(T x, T y)           { return eval(Point2<T>{ x, y }); }
    template<typename T> PM_INLINE T eval(T x, T y, T z)      { return eval(Point3<T>{ x, y, z }); }
    template<typename T> PM_INLINE T eval(T x, T y, T z, T w) { return eval(Point4<T>{ x, y, z, w }); }

  };

  // 2D Noise Function
  template<typename T>
  T Perlin::PerlinGenerator::eval(Point2<T> &p)
  {
    Point2<T> floored = PointMath::map2(p, PointMath::MapOps::FastFloor);
    Point2<int8_t> nearCorner = PointMath::convert2<T, int8_t>(floored).result;
    Point2<int8_t> farCorner = PointMath::add2(nearCorner, PointMath::one2<int8_t>());
    Point2<T> nearDistance = PointMath::sub2(p, floored);
    Point2<T> farDistance = PointMath::sub2(nearDistance, PointMath::one2<T>());

    T f00 = surflet2(permutationTable, nearCorner, nearDistance);
    T f10 = surflet2(permutationTable, Point2<int8_t>{farCorner[0], nearCorner[1]}, Point2<T>{farDistance[0], nearDistance[1]});
    T f01 = surflet2(permutationTable, Point2<int8_t>{nearCorner[0], farCorner[1]}, Point2<T>{nearDistance[0], farDistance[1]});
    T f11 = surflet2(permutationTable, farCorner, farDistance);

    return (f00 + f10 + f01 + f11) * static_cast<T>(3.1604938271604937);
  }

  // 3D Noise function
  template<typename T>
  T Perlin::PerlinGenerator::eval(Point3<T> &p)
  {
    Point3<T> floored = PointMath::map3(p, PointMath::MapOps::FastFloor);
    Point3<int8_t> nearCorner = PointMath::convert3<T, int8_t>(floored).result;
    Point3<int8_t> farCorner = PointMath::add3(nearCorner, PointMath::one3<int8_t>());
    Point3<T> nearDistance = PointMath::sub3(p, floored);
    Point3<T> farDistance = PointMath::sub3(nearDistance, PointMath::one3<T>());

    T f000 = surflet3( permutationTable
                     , nearCorner
                     , nearDistance);
    T f100 = surflet3( permutationTable
                     , Point3<int8_t>{farCorner[0], nearCorner[1], nearCorner[2]}
                     , Point3<T>{farDistance[0], nearDistance[1], nearDistance[2]});
    T f010 = surflet3( permutationTable
                     , Point3<int8_t>{nearCorner[0], farCorner[1], nearCorner[2]}
                     , Point3<T>{nearDistance[0], farDistance[1], nearDistance[2]});
    T f110 = surflet3( permutationTable
                     , Point3<int8_t>{farCorner[0], farCorner[1], nearCorner[2]}
                     , Point3<T>{farDistance[0], farDistance[1], nearDistance[2]});
    T f001 = surflet3( permutationTable
                     , Point3<int8_t>{nearCorner[0], nearCorner[1], farCorner[2]}
                     , Point3<T>{nearDistance[0], nearDistance[1], farDistance[2]});
    T f101 = surflet3( permutationTable
                     , Point3<int8_t>{farCorner[0], nearCorner[1], farCorner[2]}
                     , Point3<T>{farDistance[0], nearDistance[1], farDistance[2]});
    T f011 = surflet3( permutationTable
                     , Point3<int8_t>{nearCorner[0], farCorner[1], farCorner[2]}
                     , Point3<T>{nearDistance[0], farDistance[1], farDistance[2]});
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
    Point4<int8_t> farCorner = PointMath::add4(nearCorner, PointMath::one4<int8_t>());
    Point4<T> nearDistance = PointMath::sub4(p, floored);
    Point4<T> farDistance = PointMath::sub4(nearDistance, PointMath::one4<T>());

    T f0000 = surflet4( permutationTable
                      , nearCorner
                      , nearDistance);
    T f1000 = surflet4( permutationTable
                      , Point4<int8_t>{farCorner[0], nearCorner[1], nearCorner[2], nearCorner[3]}
                      , Point4<T>{farDistance[0], nearDistance[1], nearDistance[2], nearDistance[3]});
    T f0100 = surflet4( permutationTable
                      , Point4<int8_t>{nearCorner[0], farCorner[1], nearCorner[2], nearCorner[3]}
                      , Point4<T>{nearDistance[0], farDistance[1], nearDistance[2], nearDistance[3]});
    T f1100 = surflet4( permutationTable
                      , Point4<int8_t>{farCorner[0], farCorner[1], nearCorner[2], nearCorner[3]}
                      , Point4<T>{farDistance[0], farDistance[1], nearDistance[2], nearDistance[3]});
    T f0010 = surflet4( permutationTable
                      , Point4<int8_t>{nearCorner[0], nearCorner[1], farCorner[2], nearCorner[3]}
                      , Point4<T>{nearDistance[0], nearDistance[1], farDistance[2], nearDistance[3]});
    T f1010 = surflet4( permutationTable
                      , Point4<int8_t>{farCorner[0], nearCorner[1], farCorner[2], nearCorner[3]}
                      , Point4<T>{farDistance[0], nearDistance[1], farDistance[2], nearDistance[3]});
    T f0110 = surflet4( permutationTable
                      , Point4<int8_t>{nearCorner[0], farCorner[1], farCorner[2], nearCorner[3]}
                      , Point4<T>{nearDistance[0], farDistance[1], farDistance[2], nearDistance[3]});
    T f1110 = surflet4( permutationTable
                      , Point4<int8_t>{farCorner[0], farCorner[1], farCorner[2], nearCorner[3]}
                      , Point4<T>{farDistance[0], farDistance[1], farDistance[2], nearDistance[3]});
    T f0001 = surflet4( permutationTable
                      , Point4<int8_t>{nearCorner[0], nearCorner[1], nearCorner[2], farCorner[3]}
                      , Point4<T>{nearDistance[0], nearDistance[1], nearDistance[2], farDistance[3]});
    T f1001 = surflet4( permutationTable
                      , Point4<int8_t>{farCorner[0], nearCorner[1], nearCorner[2], farCorner[3]}
                      , Point4<T>{farDistance[0], nearDistance[1], nearDistance[2], farDistance[3]});
    T f0101 = surflet4( permutationTable
                      , Point4<int8_t>{nearCorner[0], farCorner[1], nearCorner[2], farCorner[3]}
                      , Point4<T>{nearDistance[0], farDistance[1], nearDistance[2], farDistance[3]});
    T f1101 = surflet4( permutationTable
                      , Point4<int8_t>{farCorner[0], farCorner[1], nearCorner[2], farCorner[3]}
                      , Point4<T>{farDistance[0], farDistance[1], nearDistance[2], farDistance[3]});
    T f0011 = surflet4( permutationTable
                      , Point4<int8_t>{nearCorner[0], nearCorner[1], farCorner[2], farCorner[3]}
                      , Point4<T>{nearDistance[0], nearDistance[1], farDistance[2], farDistance[3]});
    T f1011 = surflet4( permutationTable
                      , Point4<int8_t>{farCorner[0], nearCorner[1], farCorner[2], farCorner[3]}
                      , Point4<T>{farDistance[0], nearDistance[1], farDistance[2], farDistance[3]});
    T f0111 = surflet4( permutationTable
                      , Point4<int8_t>{nearCorner[0], farCorner[1], farCorner[2], farCorner[3]}
                      , Point4<T>{nearDistance[0], farDistance[1], farDistance[2], farDistance[3]});
    T f1111 = surflet4( permutationTable
                      , farCorner
                      , farDistance);

    return (f0000 + f1000 + f0100 + f1100 
          + f0010 + f1010 + f0110 + f1110 
          + f0001 + f1001 + f0101 + f1101 
          + f0011 + f1011 + f0111 + f1111) * static_cast<T>(4.424369240215691);
  }

}
