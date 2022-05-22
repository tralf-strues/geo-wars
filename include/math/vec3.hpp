/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file vec3.hpp
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

#include "math/vec2.hpp"
#include <assert.h>
#include <math.h>

namespace gwars {

template<typename T>
struct Vec3
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
        };

        T elems[3];
    };

    Vec3(const Vec2<T>& vector, T z = 1) : x(vector.x), y(vector.y), z(z) {}
    Vec3(const T& x = 0, const T& y = 0, const T& z = 0) : x(x), y(y), z(z) {}

    T& operator[](size_t element)
    {
        assert(element < 3);
        return elems[element];
    }

    const T& operator[](size_t element) const
    {
        assert(element < 3);
        return elems[element];
    }

    Vec3& operator+=(const Vec3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vec3& operator*=(const Vec3& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    Vec3& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vec3& operator/=(T scalar)
    {
        assert(scalar != 0);

        x /= scalar;
        y /= scalar;
        z /= scalar;

        return *this;
    }

    operator Vec2<T>() const { return Vec2<T>(x, y); }
};

template<typename T>
Vec3<T> operator+(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    Vec3<T> sum(lhs);
    sum += rhs;
    return sum;
}

template<typename T>
Vec3<T> operator-(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    Vec3<T> dif(lhs);
    dif -= rhs;
    return dif;
}

template<typename T>
Vec3<T> operator*(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    Vec3<T> mul(lhs);
    mul *= rhs;
    return mul;
}

template<typename T>
Vec3<T> operator*(const Vec3<T>& vector, T scalar)
{
    Vec3<T> mul(vector);
    mul *= scalar;
    return mul;
}

template<typename T>
Vec3<T> operator*(T scalar, const Vec3<T>& vector)
{
    return vector * scalar;
}

template<typename T>
Vec3<T> operator/(const Vec3<T>& vector, T scalar)
{
    Vec3<T> ratio(vector);
    ratio /= scalar;
    return ratio;
}

template<typename T>
Vec3<T> operator-(const Vec3<T>& vector)
{
    return vector * (-1.0f);
}

template<typename T>
float lengthSquare(const Vec3<T>& vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

template<typename T>
float length(const Vec3<T>& vector)
{
    return sqrtf(lengthSquare(vector));
}

template<typename T>
Vec3<T> normalize(const Vec3<T>& vector)
{
    float len = length(vector);
    assert(len != 0);

    return vector / len;
}

template<typename T>
T dot(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename T>
Vec3<T> cross(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    Vec3<T> product;

    product.x = lhs.y * rhs.z - lhs.z * rhs.y;
    product.y = lhs.z * rhs.x - lhs.x * rhs.z;
    product.z = lhs.x * rhs.y - lhs.y * rhs.x;

    return product;
}

using Vec3u = Vec3<uint32_t>;
using Vec3i = Vec3<int32_t>;
using Vec3f = Vec3<float>;

} // namespace gwars