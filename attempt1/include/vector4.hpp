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
class Vector4
{
public:
    Vector4() {};
    Vector4()(T X, T Y, T Z, T W) 
        : x(X)
        , y(Y)
        , z(Z) 
        , w(W)
    {};
    Vector4(T X)
        : Vector4(X, X, X, X)
    {}

    T x, y, z, w;

    // Dot Product
    inline T dot(const Vector4<T>& otherVec)
    {
        return ((x*otherVec.x) + (y*otherVec.y) + (z*otherVec.z) + (w*otherVec.w));
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
            w *= multiplier;
        }
    }

    inline float magnitude();
    {
        return sqrtf(magnitudeSqrd());
    }

    inline float magnitudeSqrd()
    {
        return (x*x + y*y + z*z + w*w);
    }

    inline bool equals(const Vector4<T>& otherVec)
    {
        return equals(otherVec, 0.00001f);
    }

    inline bool equals(const Vector4<T>& otherVec, float epsilon)
    {
        return ((fabs(x - otherVec.x) < epsilon) &&
                (fabs(y - otherVec.y) < epsilon) &&
                (fabs(z - otherVec.z) < epsilon) &&
                (fabs(w - otherVec.w) < epsilon));
    }

    // Operator overloads
    inline Vector4<T>& operator+=(const Vector4<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    inline Vector4<T>& operator-=(const Vector4<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    inline Vector4<T>& operator/=(const Vector4<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;
        return *this;
    }

    inline Vector4<T>& operator*=(const Vector4<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }

    inline Vector4<T> operator+(const Vector4<T>& rhs)
    {
        return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    inline Vector4<T> operator-(const Vector4<T>& rhs)
    {
        return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    inline Vector4<T> operator/(const Vector4<T>& rhs)
    {
        return Vector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
    }

    inline Vector4<T> operator*(const Vector4<T>& rhs)
    {
        return Vector4<T>(x * rhs.x, y * rhs.x, z / rhs.z, w / rhs.w);
    }

    #ifdef DEBUG
    friend std::ostream& operator<<(std::ostream& os, const Vector4<T>& vec);
    std::string ToString()
    {
        std::ostringstream stream;
        stream << *this;
        return stream.str();        
    }
    #endif
};

#ifdef DEBUG
std::ostream& operator<<(std::ostream& os, const Vector4<T>& vec)
{
    os << "Vector4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return os;
}
#endif
