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
#include <vector>

namespace gwars {

struct Polygon
{
    struct Vertex
    {
        Vec2f vertex;
        bool  isBreak{false};

        Vertex(const Vec2f& vertex = Vec2f{0, 0}, bool isBreak = false) : vertex(vertex), isBreak(isBreak) {}
    };

    std::vector<Vertex> vertices;
    Color               color{0xFFFFFFFF};
    float               thickness{1};

    static Polygon
    createLine(Vec2f from = Vec2f(0, 0), Vec2f to = Vec2f(1, 0), Color color = 0xFFFFFFFF, float thickness = 1);

    static Polygon createTriangle(Vec2f v0        = Vec2f(-0.5, -0.5),
                                  Vec2f v1        = Vec2f(0.5, -0.5),
                                  Vec2f v2        = Vec2f(0, 0.5),
                                  Color color     = 0xFFFFFFFF,
                                  float thickness = 1);

    static Polygon createQuad(Vec2f v0        = Vec2f(-0.5, -0.5),
                              Vec2f v1        = Vec2f(0.5, -0.5),
                              Vec2f v2        = Vec2f(0.5, 0.5),
                              Vec2f v3        = Vec2f(-0.5, 0.5),
                              Color color     = 0xFFFFFFFF,
                              float thickness = 1);
};

} // namespace gwars