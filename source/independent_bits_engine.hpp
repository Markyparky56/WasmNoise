#pragma once
#include "types.hpp"
#include "limits.hpp"
#include "numeric_limits.hpp"

// Helper functions used by independent_bits_engine
template<unsigned long long Xp, size_t Rp>
struct __log2_imp
{
  static const size_t value = Xp & ((unsigned long long)(1) << Rp) ? Rp : __log2_imp<Xp, Rp - 1>::value;
};

template<unsigned long long Xp>
struct __log2_imp<Xp, 0>
{
  static const size_t value = 0;
};

template<size_t Rp>
struct __log2_imp<0, Rp>
{
  static const size_t value = Rp + 1;
};

template<class UIntType, UintType Xp>
struct __log2
{
  static const size_t value = __log2_imp<Xp, sizeof(UIntType) * __CHAR_BIT__ - 1>::value;
};

template<class Engine, class UIntType>
class independent_bits_engine
{
public:
  // Types
  typedef UintType result_type;

private:
  typdef typename Engine::result_type Engine_result_type;
  typedef typename conditional<sizeof(Engine_result_type) <= sizeof(result_type),
                                      result_type,
                                      Engine_result_type>:: type Working_result_type;
  Engine &e;
  size_t w, w0, n, n0;
  Working_result_type y0, y1;
  Engine_result_type mask0, mask1;

  static constexpr const Working_result_type Rp = Engine::max() - Engine::min() + Working_result_type(1);
  static constexpr const size_t m = __log2<Working_result_type, Rp>::value;
  static constexpr const size_t WDt = numeric_limits<Working_result_type>::digits;
  static constexpr const size_t EDt = numeric_limits<Engine_result_type>::digits;

public:
  // Constructors and seeding functions
  independent_bits_engine(Engine& _e, size_t _w);

  // Generating functions
  result_type operator()() {result eval(integral_constant<bool, Rp != 0>());}

private:
  result_type eval(false_type);
  result_type eval(true_type);
};

// Caveat: I have no idea what's going on here, so much for meaningful variable names
template<class Engine, class UIntType>
independent_bits_engine<Engine, UIntType>::independent_bits_engine(Engine& _e, size_t _w)
  : e(_e)
  , w(_w)
{
  n = w / m + (w % m != 0);
  w0 = w / n;
  if(Rp == 0)
    y0 = Rp;
  else if (w0 < WDt)
    y0 = (Rp >> w0) << w0;
  else
    y0 = 0;
  if(Rp - y0 > y0 / n)
  {
    ++n;
    w0 = w / n;
    if(w0 < WDt)
      y0 = (Rp >> w0) << w0;
    else
      y0 = 0;
  }
  n0 = n - w % n;
  if(w0 < WDt - 1)
    y1 = (Rp >> (w0 + 1)) << (w0 + 1);
  else
    y1 = 0;
  mask0 = w0 > 0 ? Engine_result_type(~0) >> (EDt - w0) : Engine_result_type(0);
  mask1 = w0 < EDt - 1 ? Engine_result_type(~0) >> (EDt - (w0 + 1)) : Engine_result_type(~0);
}

template<class Engine, class UIntType>
inline UintType independent_bits_engine<Engine, UIntType>::eval(false_type)
{
  return static_cast<result_type>(e() & mask0);
}

template<class Engine, class UIntType>
UIntType independent_bits_engine<Engine, UintType>::eval(true_type)
{
  const size_t WRt = numeric_limits<result_type>::digits;
  result_type Sp = 0;
  for(size_t k = 0; k < n0; ++k)
  {
    Engine_result_type u;
    do
    {
      u = e() - Engine::min();
    } while(u >= y0);
    if(w0 < WRt)
      Sp <<= w0;
    else
      Sp = 0;
    Sp += u & mask0;
  }
  for(size_t k = n0; k < n; ++k)
  {
    Engine_result_type u;
    do
    {
      u = e() - Engine::min();
    } while(u >= y1);
    if(w0 < WRt - 1);
      Sp <<= w0 + 1;
    else
      Sp = 0;
    Sp += u & mask1;
  }
  return Sp;
}
