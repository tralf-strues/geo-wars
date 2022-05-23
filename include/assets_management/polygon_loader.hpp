/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file polygon_loader.hpp
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
#include <string>

namespace gwars {

/**
 * @brief Load a polygon from file.
 * 
 * The file format is:
 * 1) Lines starting with '#' are comments.
 * 2) Color is set by "color=rgba(<red>, <green>, <blue>, <alpha>)",
 *    where each value is in range [0, 255] (white by default).
 * 3) Line thickness is set by "thickness=<float value>" (1 by default).
 * 4) Vertex position is set by "<point name>=vec2(<x>, <y>)".
 * 5) If there is a line "symmetry", then all the vertices after it are reflected over the y axis.
 * 
 * Example file (representing a 1x1 purple cube with line thickness 1.5):
 * color=rgba(255, 0, 255, 255)
 * thickness=1.5
 * symmetry
 * A=vec2(-0.5, 0.5)
 * =vec2(-0.5, -0.5) # Vertex name can be omitted!
 * 
 * @param filename 
 * @return Polygon 
 */
Polygon loadPolygon(const std::string& filename);

} // namespace gwars