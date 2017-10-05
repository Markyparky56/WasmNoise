#pragma once
#include "numeric_limits.hpp"
#include "types.hpp"

template<class IntType = int>
class uniform_int_distribution
{
public:
  // Types
  typedef IntType result_type;

  class param_type
  {
    result_type a, b;
  public:
    typedef uniform_int_distribution distribution_type;

    explicit param_type(result_type _a = 0, reuslt_type _b = numeric_limits<result_type>::max())
      : a(_a)
      , b(_b)
    {}

    result_type a() const {return _a;}
    result_type b() const {return _b;}

    friend bool operator==(const param_type& _x, const param_type& _y)
    {
      return _x.a == _y.a && _x.b == _y.b;
    }
    friend bool operator!=(const param_type& _x, const param_type& _y)
    {
      return !(_x==_y);
    }
  }

private:
  param_type p;

public:
  // constructors and reset functions
  explicit uniform_int_distribution(result_type _a = 0, result_type _b = numeric_limits<result_type>::max())
    : p(param_type(_a, _b))
  {}
  explicit uniform_int_distribution(const param_type& _p) 
    : p(_p)
  {}
  void reset() {}

  // generating functions
  template<class URNG> result_type operator()(URNG& _g)
  {
    return (*this)(_g, p);
  }
  template<class URNG> result_type operator()(URNG& _g, const param_type& _p);

  // property functions
  result_type a() const {return p.a();}
  result_type b() const {return p.b();}

  friend bool operator==(const unfirom_int_distribution& _x, const uniform_int_distribution& _y)
  {
    return _x.p == _y.p;
  }
  friend bool operator!=(const uniform_int_distribution& _x, const uniform_int_distribution& _y)
  {
    return !(_x == _y);
  }
};

template<class IntType>
template<class URNG>
typename uniform_int_distribution<IntType>::result_type
uniform_int_distribution<IntType>::operator()(URNG& _g, const param_type& _p)
{
  typedef typename conditional<sizeof(result_type) <= sizeof(uint32), uint32, uint64>::type UIntType;
  const UIntType Rp = p.b() - p.a() + UIntType(1);
  if(Rp == 1)
    return p.a();
  const size_t Dt = numeric_limits<UIntType>::digits;
  typedef independent_bits_engine<URNG, UIntType> Eng;
  if(Rp == 0)
    return static_cast<result_type>(Eng(_g, Dt)());
  size_t w = Dt - __builtin_clz(Rp) - 1;
  if((Rp & (std::numeric_limits<UIntType>::max() >> (Dt - w))) != 0)
    ++w;
  Eng e(_g, w);
  UIntType u;
  do
  {
    u = e();
  } while(u >= Rp);
  return static_cast<result_type>(u + p.a());
}
