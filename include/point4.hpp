#pragma once
template<typename T>
struct Point4
{
private:
  T arr[4];
public:
  Point4() {};
  Point4(T X, T Y, T Z, T W) : arr{ X, Y, Z, W } {};
  Point4(T X) : arr{ X, X, X, X } {};
        T& operator[]( std::size_t i )       { return arr[i]; }
  const T& operator[]( std::size_t i ) const { return arr[i]; }
};
