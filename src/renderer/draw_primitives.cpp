/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file draw_primitives.cpp
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

#include "renderer/draw_primitives.hpp"

namespace gwars {

Polygon Polygon::createLine(Vec2f from, Vec2f to, Color color, float thickness)
{
    Polygon polygon;
    polygon.vertices.push_back(from);
    polygon.vertices.push_back(to);

    polygon.color     = color;
    polygon.thickness = thickness;

    return polygon;
}

Polygon Polygon::createTriangle(Vec2f v0, Vec2f v1, Vec2f v2, Color color, float thickness)
{
    Polygon polygon;
    polygon.vertices.push_back(v0);
    polygon.vertices.push_back(v1);
    polygon.vertices.push_back(v2);

    polygon.color     = color;
    polygon.thickness = thickness;

    return polygon;
}

Polygon Polygon::createQuad(Vec2f v0, Vec2f v1, Vec2f v2, Vec2f v3, Color color, float thickness)
{
    Polygon polygon;
    polygon.vertices.push_back(v0);
    polygon.vertices.push_back(v1);
    polygon.vertices.push_back(v2);
    polygon.vertices.push_back(v3);

    polygon.color     = color;
    polygon.thickness = thickness;

    return polygon;
}

} // namespace gwars