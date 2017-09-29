#pragma once
#include <cstddef>

template<typename T>
struct Point2
{
private:
  T arr[2];
public:
  Point2(){};
  Point2(T X, T Y) : arr{ X, Y } {};
  Point2(T X) : arr{ X, X } {};
        T& operator[]( std::size_t i )       { return arr[i]; }
  const T& operator[]( std::size_t i ) const { return arr[i]; }
};
