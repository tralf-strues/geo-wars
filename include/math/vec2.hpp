/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file vec2.hpp
 * @date 2022-05-20
 *
 * The MIT License (MIT)
 * Copyright (c) 2022 Nikita Mochalov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <assert.h>
#include <inttypes.h>
#include <math.h>

namespace gwars {

template<typename T>
struct Vec2
{
    union
    {
        struct
        {
            T x;
            T y;
        };

        T elems[2];
    };
    
    Vec2() = default;
    Vec2(const T& x, const T& y) : x(x), y(y) {}

    template<typename U = float>
    Vec2<int32_t>(Vec2<U> vector) : x(vector.x), y(vector.y)
    {
    }

    Vec2& operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vec2& operator*=(const Vec2& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    Vec2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
};

template<typename T>
bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template<typename T>
Vec2<T> operator+(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    Vec2<T> sum(lhs);
    sum += rhs;
    return sum;
}

template<typename T>
Vec2<T> operator-(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    Vec2<T> dif(lhs);
    dif -= rhs;
    return dif;
}

template<typename T>
Vec2<T> operator*(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    Vec2<T> mul(lhs);
    mul *= rhs;
    return mul;
}

template<typename T>
Vec2<T> operator*(const Vec2<T>& vector, T scalar)
{
    Vec2<T> mul(vector);
    mul *= scalar;
    return mul;
}

template<typename T>
Vec2<T> operator*(T scalar, const Vec2<T>& vector)
{
    return vector * scalar;
}

template<typename T>
Vec2<T> operator/(const Vec2<T>& vector, T scalar)
{
    Vec2<T> ratio(vector);
    ratio /= scalar;
    return ratio;
}

template<typename T>
Vec2<T> operator-(const Vec2<T>& vector)
{
    return vector * (-1.0f);
}

template<typename T>
Vec2<T> componentMultiply(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return {lhs.x * rhs.x, lhs.y * rhs.y};
}

template<typename T>
float lengthSquare(const Vec2<T>& vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

template<typename T>
float length(const Vec2<T>& vector)
{
    return sqrtf(lengthSquare(vector));
}

template<typename T>
void rotate(Vec2<T>& vector, float angle)
{
    float sina = sinf(angle);
    float cosa = cosf(angle);

    float x = vector.x;
    float y = vector.y;

    vector.x = x * cosa - y * sina;
    vector.y = x * sina + y * cosa;
}

template<typename T>
Vec2<T> getNormal(const Vec2<T>& vector)
{
    float length = length(vector);
    if (length == 0.0f)
    {
        return Vec2<T>{0, 0};
    }

    return normalize(Vec2<T>{-vector.y / length, vector.x / length});
}

template<typename T>
Vec2<T> normalize(const Vec2<T>& vector)
{
    float len = length(vector);
    if (len == 0) { return Vec2<T>(0, 0); }

    return vector / len;
}

template<typename T>
T dot(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

using Vec2u = Vec2<uint32_t>;
using Vec2i = Vec2<int32_t>;
using Vec2f = Vec2<float>;

} // namespace gwars