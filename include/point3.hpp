#pragma once
template<typename T>
struct Point3
{
private:
  T arr[3];
public:
  Point3() {};
  Point3(T X, T Y, T Z) : arr{ X, Y, Z } {};
  Point3(T X) : arr{ X, X, X } {};
        T& operator[]( std::size_t i )       { return arr[i]; }
  const T& operator[]( std::size_t i ) const { return arr[i]; }
};
