#pragma once
#include "points.hpp"
#include "xoroshiro128plus.hpp"
#include <cstdint>
#include <array>
using std::uint8_t;

// The classic table, from Ken Perlin's reference perlin implementation is used as the 
// starting value for the permutation array if no seed is provided at initialisation
class PermutationTable
{
  std::array<uint8_t, 256> perm;

  static std::array<uint8_t, 256> newPermTable(xoroshiro128plus &rng)
  {
    std::array<uint8_t, 256> tmpPerm;

    // Fill the temporary table with values from 0 till 255
    for (int i = 0; i < 256; i++)
    {
      tmpPerm[i] = i;
    }

    // Then shuffle those values
    rng.shuffle(tmpPerm);

    return tmpPerm;
  }

public:
  PermutationTable()
    : perm
      {
        151, 160,137, 91,90, 15,131, 13,201,95,96,53,194,233,7,225,140,36,103,30,69,
        142,8,99,37,240,21,10, 23,190, 6,148,247,120,234,75, 0,26,197,62,94,252,219,
        203,117,35,11,32,57,177,33, 88,237,149,56,87,174,20,125,136,171,168, 68,175,
        74,165,71,134,139,48, 27,166, 77,146,158,231, 83,111,229,122,60,211,133,230,
        220,105,92,41,55,46,245,40,244, 102,143,54, 65,25,63,161, 1,216, 80, 73,209,
        76,132,187,208, 89, 18,169,200,196, 135,130,116,188,159, 86,164,100,109,198,
        173,186, 3,64,52,217,226,250,124,123, 5,202, 38,147,118,126,255, 82, 85,212,
        207,206,59,227,47,16,58,17,182,189,28,42, 223,183,170,213,119,248,152, 2,44,
        154,163, 70,221,153,101,155,167, 43,172,9, 129,22,39,253, 19, 98,108,110,79,
        113,224,232,178,185, 112,104,218,246,97,228, 251,34,242,193,238,210,144, 12,
        191,179,162,241, 81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,
        157,184, 84,204,176,115,121,50,45,127, 4,150,254, 138,236,205,93,222,114,67,
        29,24,72,243,141,128,195,78,66,215,61,156,180
      }
  {}
  PermutationTable(std::array<uint8_t, 256> &p)
    : perm(p)
  {}
  PermutationTable(uint64_t seed)
    : perm(newPermTable(xoroshiro128plus(seed)))
  {}


  inline uint8_t get1(int8_t i)
  {
    // mask and cast ensures value is within array limits
    return perm[static_cast<uint8_t>(i & 0xff)];
  }

  inline uint8_t get2(Point2<int8_t> pos)
  {
    return perm[get1(pos[0]) ^ static_cast<uint8_t>((pos[1] & 0xff))];
  }

  inline uint8_t get3(Point3<int8_t> pos)
  {
    return perm[get2({ pos[0], pos[1] }) ^ static_cast<uint8_t>((pos[2] & 0xff))];
  }

  inline uint8_t get4(Point4<int8_t> pos)
  {
    return perm[get3({ pos[0], pos[1], pos[2] }) ^ static_cast<uint8_t>((pos[3] & 0xff))];
  }

  static PermutationTable NewTable(uint64_t seed)
  {
    xoroshiro128plus rng(seed);

    // Return a new PermutationTable initialised with a new shuffled table
    return PermutationTable(newPermTable(rng));
  }

};
