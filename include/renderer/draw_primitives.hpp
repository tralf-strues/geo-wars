/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file draw_primitives.hpp
 * @date 2022-05-22
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

#include "math/mat3.hpp"
#include "renderer/color.hpp"

namespace gwars {

struct Line
{
    Vec2f from;
    Vec2f to;
    Color color;
    float thickness;

    Line(Vec2f from = Vec2f(0, 0), Vec2f to = Vec2f(1, 0), Color color = 0xFFFFFFFF, float thickness = 1)
        : from(from), to(to), color(color), thickness(thickness)
    {
    }
};

struct Triangle
{
    Vec2f vertices[3];
    Color color;
    float thickness;

    Triangle(Vec2f v0        = Vec2f(-0.5, -0.5),
             Vec2f v1        = Vec2f(0.5, -0.5),
             Vec2f v2        = Vec2f(0, 0.5),
             Color color     = 0xFFFFFFFF,
             float thickness = 1)
        : color(color), thickness(thickness)
    {
        vertices[0] = v0;
        vertices[1] = v1;
        vertices[2] = v2;
    }
};

struct Quad
{
    Vec2f vertices[4];
    Color color;
    float thickness;

    Quad(Vec2f v0        = Vec2f(-0.5, -0.5),
         Vec2f v1        = Vec2f(0.5, -0.5),
         Vec2f v2        = Vec2f(0.5, 0.5),
         Vec2f v3        = Vec2f(-0.5, 0.5),
         Color color     = 0xFFFFFFFF,
         float thickness = 1)
        : color(color), thickness(thickness)
    {
        vertices[0] = v0;
        vertices[1] = v1;
        vertices[2] = v2;
        vertices[3] = v3;
    }
};

} // namespace gwars