/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file polygon_loader.cpp
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

#include "assets_management/polygon_loader.hpp"

#include <fstream>
#include <iostream>

namespace gwars {

Polygon loadPolygon(const std::string& filename)
{
    printf("Loading polygon from \"%s\"\n", filename.c_str());

    std::ifstream stream(filename);
    if (stream.fail())
    {
        printf("Failed to open file \"%s\"\n", filename.c_str());
        return Polygon{};
    }

    Polygon polygon;
    polygon.color     = 0xFFFFFFFF;
    polygon.thickness = 1;

    int32_t symmetryStart = -1;

    std::string curLine;
    int32_t     curPoint{0};
    while (std::getline(stream, curLine))
    {
        uint32_t rgba[4];
        float    thickness;
        size_t   vertexPosStart;
        float    vertexPos[2];

        if (curLine.find("symmetry") != curLine.npos)
        {
            symmetryStart = curPoint;
        }
        else if (std::sscanf(curLine.c_str(), "color=rgba(%u, %u, %u, %u)", &rgba[0], &rgba[1], &rgba[2], &rgba[3])
                 == 4)
        {
            polygon.color = Color(rgba[0], rgba[1], rgba[2], rgba[3]);
        }
        else if (std::sscanf(curLine.c_str(), "thickness=%f", &thickness) == 1)
        {
            polygon.thickness = thickness;
        }
        else if (((vertexPosStart = curLine.find("=vec2(")) != curLine.npos)
                 && (std::sscanf(curLine.substr(vertexPosStart).c_str(), "=vec2(%f,%f)", &vertexPos[0], &vertexPos[1])
                     == 2))
        {
            polygon.vertices.push_back(Vec2f(vertexPos[0], vertexPos[1]));
            ++curPoint;
        }
    }

    if (curPoint == 0)
    {
        printf("No vertices found in \"%s\"\n", filename.c_str());
    }

    if (symmetryStart != -1)
    {
        int32_t endVertex = polygon.vertices.size();
        for (int32_t vertex = endVertex - 1; vertex >= symmetryStart; --vertex)
        {
            polygon.vertices.push_back(Vec2f(-polygon.vertices[vertex].vertex.x, polygon.vertices[vertex].vertex.y));
        }
    }

    printf("Loaded polygon from \"%s\" (vertices_count = %lu)\n", filename.c_str(), polygon.vertices.size());

    return polygon;
}

} // namespace gwars