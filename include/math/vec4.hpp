/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file vec4.hpp
 * @date 2022-05-23
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

#include "math/vec3.hpp"
#include <assert.h>
#include <math.h>

namespace gwars {

template<typename T>
struct Vec4
{
    union
    {
        struct
        {
            union
            {
                T x;
                T r;
            };

            union
            {
                T y;
                T g;
            };

            union
            {
                T z;
                T b;
            };

            union
            {
                T w;
                T a;
            };  
        };

        T elems[4];
    };

    Vec4(const Vec3<T>& vector, T w = 1) : x(vector.x), y(vector.y), z(vector.z), w(w) {}
    Vec4(const T& x = 0, const T& y = 0, const T& z = 0, const T& w = 1) : x(x), y(y), z(z), w(w) {}

    T& operator[](size_t element)
    {
        assert(element < 4);
        return elems[element];
    }

    const T& operator[](size_t element) const
    {
        assert(element < 4);
        return elems[element];
    }

    Vec4& operator+=(const Vec4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vec4& operator-=(const Vec4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vec4& operator*=(const Vec4& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }

    Vec4& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Vec4& operator/=(T scalar)
    {
        assert(scalar != 0);

        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;

        return *this;
    }

    operator Vec3<T>() const { return Vec3<T>(x, y, z); }
};

template<typename T>
Vec4<T> operator+(const Vec4<T>& lhs, const Vec4<T>& rhs)
{
    Vec4<T> sum(lhs);
    sum += rhs;
    return sum;
}

template<typename T>
Vec4<T> operator-(const Vec4<T>& lhs, const Vec4<T>& rhs)
{
    Vec4<T> dif(lhs);
    dif -= rhs;
    return dif;
}

template<typename T>
Vec4<T> operator*(const Vec4<T>& lhs, const Vec4<T>& rhs)
{
    Vec4<T> mul(lhs);
    mul *= rhs;
    return mul;
}

template<typename T>
Vec4<T> operator*(const Vec4<T>& vector, T scalar)
{
    Vec4<T> mul(vector);
    mul *= scalar;
    return mul;
}

template<typename T>
Vec4<T> operator*(T scalar, const Vec4<T>& vector)
{
    return vector * scalar;
}

template<typename T>
Vec4<T> operator/(const Vec4<T>& vector, T scalar)
{
    Vec4<T> ratio(vector);
    ratio /= scalar;
    return ratio;
}

template<typename T>
Vec4<T> operator-(const Vec4<T>& vector)
{
    return vector * (-1.0f);
}

template<typename T>
float lengthSquare(const Vec4<T>& vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
}

template<typename T>
float length(const Vec4<T>& vector)
{
    return sqrtf(lengthSquare(vector));
}

template<typename T>
Vec4<T> normalize(const Vec4<T>& vector)
{
    float len = length(vector);
    assert(len != 0);

    return vector / len;
}

template<typename T>
T lerp(const T& lhs, const T& rhs, float t)
{
    return lhs + t * (rhs - lhs);
}

using Vec4u = Vec4<uint32_t>;
using Vec4i = Vec4<int32_t>;
using Vec4f = Vec4<float>;

} // namespace gwars