/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file mat3.hpp
 * @date 2022-05-21
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
#include <cmath>

namespace gwars {

constexpr static uint32_t MAT3_SIZE = 3;

template<typename T>
struct Mat3
{
    T elements[MAT3_SIZE * MAT3_SIZE];

    T* operator[](size_t row)
    {
        assert(row < MAT3_SIZE);
        return &elements[row * MAT3_SIZE];
    }

    const T* operator[](size_t row) const
    {
        assert(row < MAT3_SIZE);
        return &elements[row * MAT3_SIZE];
    }

    Mat3& operator+=(const Mat3& second)
    {
        for (size_t i = 0; i < MAT3_SIZE * MAT3_SIZE; i++)
        {
            elements[i] += second.elements[i];
        }

        return *this;
    }

    Mat3& operator-=(const Mat3& second)
    {
        for (size_t i = 0; i < MAT3_SIZE * MAT3_SIZE; i++)
        {
            elements[i] -= second.elements[i];
        }

        return *this;
    }

    Mat3& operator*=(const Mat3& second)
    {
        Mat3 old(*this);

        for (size_t row = 0; row < MAT3_SIZE; row++)
        {
            for (size_t col = 0; col < MAT3_SIZE; col++)
            {
                T result = 0;

                for (size_t i = 0; i < MAT3_SIZE; i++)
                {
                    result += old.elements[row * MAT3_SIZE + i] * second.elements[i * MAT3_SIZE + col];
                }

                elements[row * MAT3_SIZE + col] = result;
            }
        }

        return *this;
    }

    Mat3& operator*=(T scalar)
    {
        for (size_t i = 0; i < MAT3_SIZE * MAT3_SIZE; i++)
        {
            elements[i] *= scalar;
        }
    }
};

template<typename T>
Mat3<T> operator+(const Mat3<T>& first, const Mat3<T>& second)
{
    Mat3<T> sum(first);
    sum += second;

    return sum;
}

template<typename T>
Mat3<T> operator-(const Mat3<T>& first, const Mat3<T>& second)
{
    Mat3<T> dif(first);
    dif -= second;

    return dif;
}

template<typename T>
Mat3<T> operator*(const Mat3<T>& first, const Mat3<T>& second)
{
    Mat3<T> product(first);
    product *= second;

    return product;
}

template<typename T>
Vec3<T> operator*(const Mat3<T>& matrix, const Vec3<T>& vector)
{
    Vec3<T> product;

    for (size_t row = 0; row < MAT3_SIZE; row++)
    {
        for (size_t col = 0; col < MAT3_SIZE; col++)
        {
            product[row] += matrix[row][col] * vector[col];
        }
    }

    return product;
}

template<typename T>
Mat3<T> operator*(const Mat3<T>& matrix, T scalar)
{
    Mat3<T> product(matrix);
    product *= scalar;

    return product;
}

template<typename T>
Mat3<T> transposeMatrix(const Mat3<T>& matrix)
{
    Mat3<T> transpose;

    for (size_t row = 0; row < MAT3_SIZE; row++)
    {
        for (size_t col = 0; col < MAT3_SIZE; col++)
        {
            transpose[row][col] = matrix[col][row];
        }
    }

    return transpose;
}

template<typename T>
Mat3<T> identityMatrix(T diagonal)
{
    Mat3<T> matrix;

    for (size_t row = 0; row < MAT3_SIZE; row++)
    {
        for (size_t col = 0; col < MAT3_SIZE; col++)
        {
            matrix[row][col] = diagonal;
        }
    }

    return matrix;
}

/**
 * @brief Calculate the determinant of the matrix.
 *
 * @param matrix
 * @return Determinant.
 */
float determinant(const Mat3<float>& matrix);

/**
 * @brief Create a rotation matrix.
 *
 * @param angle
 *
 * @return Rotation matrix.
 */
Mat3<float> rotationMatrix(float angle);

/**
 * @brief Create a scaling matrix.
 *
 * @param scale
 *
 * @return Scale matrix.
 */
Mat3<float> scaleMatrix(Vec2<float> scale);

/**
 * @brief Create a translation matrix (to translate vertices by dx, dy).
 *
 * @param translation
 *
 * @return Translation matrix.
 */
Mat3<float> translationMatrix(Vec2<float> translation);

using Mat3f = Mat3<float>;

} // namespace gwars