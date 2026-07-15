// floason (C) 2026
// Licensed under the MIT License.

// Nuke's math header, which while is basic, decouples itself from the rest of SDL, 
// and should be optimised by modern compilers anyway.

#pragma once

#include <cmath>
#include <cassert>

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

// Mathematical constants.
struct pi
{
    static constexpr long double d = 3.14159265358979323846;
    static constexpr float f = static_cast<float>(d);
};
struct e
{
    static constexpr long double d = 2.71828182845904523536;
    static constexpr float f = static_cast<float>(d);
};

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
static inline T lerp(T a, T b, float t)
{
    t = min(max(t, 0.f), 1.f);
    if (a * b < 0)
        return a * (1 - t) + t * b;
    else
        return a + (b - a) * t;
}

// Remap a value from [A, B] to [C, D].
template <typename T>
static inline T remap(T val, float a, float b, float c, float d)
{
    assert(b != a && "b must not be equal to a!");
    return c + (d - c) * (val - a) / (b - a);
}

// Remap a value from [A, B] to [C, D] and clamp the result.
template <typename T>
static inline T remap_clamped(T val, float a, float b, float c, float d)
{
    assert(b != a && "b must not be equal to a!");
    float scale = min(max((val - a) / (b - a), 0.f), 1.f);
    return c + (d - c) * scale;
}

}   // namespace math

}   // namespace nuke