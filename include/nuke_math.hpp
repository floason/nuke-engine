// floason (C) 2026
// Licensed under the MIT License.

// Nuke's math header, which while is basic, decouples itself from the rest of SDL, 
// and should be optimised by modern compilers anyway.

#pragma once

#include <cmath>

namespace nuke
{

// Basic 2D vector for representing position relative to the world, velocity and angles.
struct Vector2
{
    float x;
    float y;

    Vector2(float x = 0.f, float y = 0.f) :
        x(x),
        y(y)
    {
    }
    
    Vector2 operator+(const Vector2& other)
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other)
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(const Vector2& other)
    {
        return Vector2(x * other.x, y * other.y);
    }

    Vector2 operator/(const Vector2& other)
    {
        return Vector2(x / other.x, y / other.y);
    }

    Vector2 operator+(const float offset)
    {
        return *this + Vector2(offset, offset);
    }

    Vector2 operator-(const float offset)
    {
        return *this - Vector2(offset, offset);
    }

    Vector2 operator*(const float scale)
    {
        return *this * Vector2(scale, scale);
    }

    Vector2 operator/(const float scale)
    {
        return *this / Vector2(scale, scale);
    }

    Vector2 operator-()
    {
        return Vector2(-x, -y);
    }

    float LengthSqr()
    {
        return x * x + y * y;
    }

    float Length()
    {
        return std::sqrt(LengthSqr());
    }

    float Dot(const Vector2& other)
    {
        return x * other.x + y * other.y;
    }
    
    Vector2 Normalize()
    {
        float length = Length();
        if (length > 0)
            return Vector2(x / length, y / length);
        return Vector2();
    }
};

namespace math
{

// Returns the lesser of two numbers.
template <typename T>
static inline T min(T lhs, T rhs)
{
    return (rhs > lhs) ? lhs : rhs;
}

// Returns the greater of two numbers.
template <typename T>
static inline T max(T lhs, T rhs)
{
    return (rhs > lhs) ? rhs : lhs;
}

// Get the sign of a number.
template <typename T>
static inline int get_sign(T num)
{
    return static_cast<int>(num < 0);
}

// Linear interpolation from A -> B given T weighting.
template <typename T>
static inline T lerp(T a, T b, T t)
{
    t = min(max(t, static_cast<T>(0)), static_cast<T>(1));
    if (a * b < 0)
        return a * (1 - t) + t * b;
    else
        return a + (b - a) * t;
}

}   // namespace math

}   // namespace nuke