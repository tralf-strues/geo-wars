/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file renderer.cpp
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

#include "renderer/renderer.hpp"
#include "utils/float_compare.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace gwars;

Viewport::Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) : x(x), y(y), width(width), height(height)
{
}

FrameBuffer::operator Color*() { return data; }

void RendererScenePassData::recalculateProjectionViewMatrix()
{
    projectionViewMatrix = cameraSpecs.calculateProjectionMatrix();
    projectionViewMatrix *= viewMatrix;
}

//==================================================================================================
// Utility functions
//==================================================================================================
struct Colorf
{
    Vec3f rgb; // in range [0, 255]
    float a; // in range [0, 1]

    Colorf() = default;
    Colorf(Vec3f rgb, float a) : rgb(rgb), a(a) {}
    Colorf(Color color) : rgb(color.getR(), color.getG(), color.getB()), a(color.getA() / 255.0f) {}

    explicit operator Color() const { return Color(rgb.r, rgb.g, rgb.b, a * 255); }
};

//==================================================================================================
// Basic Renderer api
//==================================================================================================
Renderer::Renderer(FrameBuffer& frameBuffer)
    : m_FrameBuffer(frameBuffer), m_Viewport{0, 0, m_FrameBuffer.width, m_FrameBuffer.height}
{
}

void Renderer::setViewport(const Viewport& viewport) { m_Viewport = viewport; }

void Renderer::beginScene(const OrthographicCameraSpecs& cameraSpecs, const Mat3f& viewMatrix)
{
    m_ScenePassData.cameraSpecs = cameraSpecs;
    m_ScenePassData.viewMatrix  = viewMatrix;
    m_ScenePassData.recalculateProjectionViewMatrix();
}

void Renderer::endScene() {}

void Renderer::clear(Color color)
{
    const uint32_t pixels = m_FrameBuffer.width * m_FrameBuffer.height;

    for (uint32_t pixel = 0; pixel < pixels; ++pixel)
    {
        m_FrameBuffer[pixel] = color;
    }
}

#include <stdio.h>
void Renderer::putPixelBlended(Vec2i pixel, Vec3f rgb, float alpha)
{
    if (cmpFloat(alpha, 0) < 0 || cmpFloat(alpha, 1) > 0)
    {
        printf("alpha = %f\n", alpha);
    }

    assert(cmpFloat(rgb.r, 0) >= 0 && cmpFloat(rgb.r, 255) <= 0);
    assert(cmpFloat(rgb.g, 0) >= 0 && cmpFloat(rgb.g, 255) <= 0);
    assert(cmpFloat(rgb.b, 0) >= 0 && cmpFloat(rgb.b, 255) <= 0);
    assert(cmpFloat(alpha, 0) >= 0 && cmpFloat(alpha, 1) <= 0);

    Colorf oldColor(getPixel(pixel));

    rgb   = rgb * alpha + oldColor.rgb * (1 - alpha);
    alpha = alpha + oldColor.a * (1 - alpha);

    putPixel(pixel, Color(Colorf{rgb, alpha}));
}

//==================================================================================================
// Line drawing
// ------------
// Modified version of https://github.com/miloyip/line SDF with AABB algorithm.
//==================================================================================================
float capsuleSDF(Vec2f pixel, Vec2f from, Vec2f to, float thickness)
{
    if (to == from)
    {
        return 0;
    }

    Vec2f fromToPixel = pixel - from;
    Vec2f line        = to - from;

    float h     = std::max(std::min(dot(fromToPixel, line) / lengthSquare(line), 1.0f), 0.0f);
    Vec2f delta = fromToPixel - line * h;

    return length(delta) - thickness;
}

void Renderer::drawLine(Vec2f msFrom, Vec2f msTo, Color color, float thickness, const Mat3f& transform)
{
    Vec2f from = ndcToFrameBuffer(m_ScenePassData.projectionViewMatrix * transform * Vec3f(msFrom));
    Vec2f to   = ndcToFrameBuffer(m_ScenePassData.projectionViewMatrix * transform * Vec3f(msTo));

    int x0 = static_cast<int>(std::floor(std::min(from.x, to.x) - thickness));
    int x1 = static_cast<int>(std::ceil(std::max(from.x, to.x) + thickness));

    int y0 = static_cast<int>(std::floor(std::min(from.y, to.y) - thickness));
    int y1 = static_cast<int>(std::ceil(std::max(from.y, to.y) + thickness));

    Colorf colorf(color);

    for (int y = y0; y <= y1; ++y)
    {
        for (int x = x0; x <= x1; ++x)
        {
            float alpha = std::max(std::min(0.5f - capsuleSDF(Vec2f(x, y), from, to, thickness), 1.0f), 0.0f);
            putPixelBlended(Vec2i(x, y), colorf.rgb, colorf.a * alpha);
        }
    }
}

void Renderer::drawPolygon(const Polygon& polygon, const Mat3f& transform)
{
    if (polygon.vertices.empty())
    {
        return;
    }

    size_t verticesCount = polygon.vertices.size();

    for (uint32_t vertex = 0; vertex < verticesCount; ++vertex)
    {
        if (polygon.vertices[vertex].isBreak || polygon.vertices[(vertex + 1) % verticesCount].isBreak)
        {
            continue;
        }

        drawLine(polygon.vertices[vertex].vertex,
                 polygon.vertices[(vertex + 1) % verticesCount].vertex,
                 polygon.color,
                 polygon.thickness,
                 transform);
    }
}