#pragma once

#ifdef _DEBUG
#include <iostream>
#include <string>
#include <sstream>
#endif //DEBUG

#include <cmath>
using std::sqrtf;
using std::fabs;

template<typename T>
class Vector3
{
public:
    Vector3() {};
    Vector3(T X, T Y, T Z) 
        : x(X)
        , y(Y)
        , z(Z) 
    {};
    Vector3(T X)
        : Vector3(X, X, X)
    {}

    T x, y, z;

    // Dot Product
    inline T dot(const Vector3<T>& otherVec)
    {
        return ((x*otherVec.x) + (y*otherVec.y) + (z*otherVec.z));
    }

    // Cross Product
    inline Vector3<T> cross(const Vector3<T>& otherVec)
    {
        return Vector3( y*otherVec.z - z*otherVec.y
                    , -(x*otherVec.z - z*otherVec.x)
                    ,   x*otherVec.y - y*otherVec.x);
    }

    inline void normalise()
    {
        float mag = magnitude();
        if(mag) // is non-zero
        {
            float multiplier = 1.f / mag;
            x *= multiplier;
            y *= multiplier;
            z *= multiplier;
        }
    }

    inline float magnitude()
    {
        return sqrtf(magnitudeSqrd());
    }

    inline float magnitudeSqrd()
    {
        return (x*x + y*y + z*z);
    }

    inline bool equals(const Vector3<T>& otherVec)
    {
        return equals(otherVec, 0.00001f);
    }

    inline bool equals(const Vector3<T>& otherVec, float epsilon)
    {
        return ((fabs(x - otherVec.x) < epsilon) &&
                (fabs(y - otherVec.y) < epsilon) &&
                (fabs(z - otherVec.z) < epsilon));
    }

    // Operator overloads
    inline Vector3<T>& operator+=(const Vector3<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    inline Vector3<T>& operator-=(const Vector3<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    inline Vector3<T>& operator/=(const Vector3<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    inline Vector3<T>& operator*=(const Vector3<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    inline Vector3<T> operator+(const Vector3<T>& rhs)
    {
        return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    inline Vector3<T> operator-(const Vector3<T>& rhs)
    {
        return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    inline Vector3<T> operator/(const Vector3<T>& rhs)
    {
        return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    inline Vector3<T> operator*(const Vector3<T>& rhs)
    {
        return Vector3<T>(x * rhs.x, y * rhs.x, z / rhs.z);
    }

    #ifdef _DEBUG
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec);
    std::string ToString()
    {
        std::ostringstream stream;
        stream << *this;
        return stream.str();        
    }
    #endif
};

#ifdef _DEBUG
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec)
{
    os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}
#endif
