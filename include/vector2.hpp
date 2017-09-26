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
class Vector2
{
public:
    Vector2() {}
    Vector2(T X, T Y) 
        : x(X)
        , y(Y) 
    {}
    T x, y;

    // Dot Product
    inline T dot(const Vector2<T>& vec2)
    {
        return ((x*vec2.x)+ (y*vec2.y));
    }

    inline void normalise()
    {
        float mag = magnitude();
        if(mag) // Non-zero
        {
            T multiplier = 1.f / mag;
            x *= multiplier;
            y *= multiplier;
        }
    }

    inline T magnitude()
    {
        return sqrtf(magnitudeSqrd());
    }

    inline T magnitudeSqrd()
    {
        return x*x + y*y;
    }

    inline bool equals(const Vector2<T>& vec2)
    {
        return equals(vec2, 0.00001f);
    }

    inline bool equals(const Vector2<T>& vec2, float epsilon)
    {
        return ((fabs(x - vec2.x) < epsilon) &&
                (fabs(y - vec2.y) < epsilon));
    }

    // Operator overloads
    inline Vector2<T>& operator+=(const Vector2<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }    

    inline Vector2<T>& operator-=(const Vector2<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    inline Vector2<T>& operator/=(const Vector2<T>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }    

    inline Vector2<T>& operator*=(const Vector2<T>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
    
    inline Vector2<T> operator+(const Vector2<T>& rhs)
    {
        return Vector2<T>(x + rhs.x, y + rhs.y);
    }

    inline Vector2<T> operator-(const Vector2<T>& rhs)
    {
        return Vector2<T>(x - rhs.x, y - rhs.y);
    }

    inline Vector2<T> operator/(const Vector2<T>& rhs)
    {
        return Vector2<T>(x / rhs.x, y - rhs.y);
    }

    inline Vector2<T> operator*(const Vector2<T>& rhs)
    {
        return Vector2<T>(x * rhs.x, y * rhs.y);
    }    

    #ifdef _DEBUG
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec);
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
std::ostream& operator<<(std::ostream& os, const Vector4<T>& vec)
{
    os << "Vector4(" << vec.x << ", " << vec.y << ")";
    return os;
}
#endif
