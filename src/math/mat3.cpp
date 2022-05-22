/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file mat3.cpp
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

#include "math/mat3.hpp"

namespace gwars {

float determinant(const Mat3<float>& matrix)
{
    return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
           matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
           matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

Mat3<float> rotationMatrix(float angle)
{
    float cos = std::cos(angle);
    float sin = std::sin(angle);

    return {{cos, -sin, 0,
             sin,  cos, 0,
               0,    0, 1}};
}

Mat3<float> scaleMatrix(Vec2<float> scale)
{
    return {{scale.x,        0, 0,
                   0,  scale.y, 0,
                   0,        0, 1}};
}

Mat3<float> translationMatrix(Vec2<float> translation)
{
    return {{1, 0, translation.x,
             0, 1, translation.y,
             0, 0,             1}};
}

Mat3<float> orthoProjectionMatrix(float left, float right, float bottom, float top)
{
    assert(right > left);
    assert(top > bottom);

    return {{2 / (right - left), 0,                  -(right + left) / (right - left),
             0,                  2 / (top - bottom), -(top + bottom) / (top - bottom),
             0,                  0,                  1}};
}

Mat3<float> orthoProjectionMatrix(float horizontal, float vertical)
{
    return orthoProjectionMatrix(-horizontal / 2, horizontal / 2, -vertical / 2, vertical / 2);
}

} // namespace gwars