/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file renderer.hpp
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

#include "math/vec3.hpp"
#include "renderer/camera.hpp"
#include "renderer/color.hpp"
#include "renderer/draw_primitives.hpp"

namespace gwars {

struct Viewport
{
    uint32_t x{0};
    uint32_t y{0};
    uint32_t width{0};
    uint32_t height{0};

    Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
};

struct FrameBuffer
{
    Color*   data{nullptr};
    uint32_t width{0};
    uint32_t height{0};

    operator Color*();
};

struct RendererScenePassData
{
    OrthographicCameraSpecs cameraSpecs;
    Mat3f                   viewMatrix;
    Mat3f                   projectionViewMatrix;

    void recalculateProjectionViewMatrix();
};

/**
 * @brief Basic 2D renderer.
 *
 * The coordinates should be given in normalized mode, that is in range
 * from 0.0 to 1.0. Then they are converted to frame buffer coordinates
 * based on the dimensions of the currently set viewport (which represents
 * the entire frame buffer by default, but can be modified via @ref
 * Renderer::setViewport() function call).
 */
class Renderer
{
public:
    Renderer(FrameBuffer& frameBuffer);

    void setViewport(const Viewport& viewport);

    void beginScene(const OrthographicCameraSpecs& cameraSpecs, const Mat3f& viewMatrix);
    void endScene();

    void clear(Color color);

    void drawLine(const Line& line, const Mat3f& transform);
    void drawTriangle(const Triangle& triangle, const Mat3f& transform);
    void drawQuad(const Quad& quad, const Mat3f& transform);

private:
    inline void putPixel(Vec2i pixel, Color color)
    {
        if (correctPixel(pixel))
        {
            m_FrameBuffer[(static_cast<int32_t>(m_FrameBuffer.height) - pixel.y - 1)
                              * static_cast<int32_t>(m_FrameBuffer.width)
                          + pixel.x]
                = color;
        }
    }

    inline Color getPixel(Vec2i pixel) const
    {
        return correctPixel(pixel) ? m_FrameBuffer[(static_cast<int32_t>(m_FrameBuffer.height) - pixel.y - 1)
                                                       * static_cast<int32_t>(m_FrameBuffer.width)
                                                   + pixel.x]
                                   : Color(0);
    }

    inline bool correctPixel(Vec2i pixel) const
    {
        return pixel.x >= 0 && pixel.x < static_cast<int32_t>(m_FrameBuffer.width) && pixel.y >= 0
               && pixel.y < static_cast<int32_t>(m_FrameBuffer.height);
    }

    /**
     * @brief Puts pixel using alpha blending.
     *
     * The exact formulas for the result color and alpha values are the following:
     * 1) resultRGB = RGB * A + oldRGB * (1 - A)
     * 2) resultA   = A       + oldA * (1 - A)
     *
     * @param pixel
     * @param rgb Non-normalized rgb vector (in range from 0 to 255)
     * @param alpha
     */
    void putPixelBlended(Vec2i pixel, Vec3f rgb, float alpha);

    inline Vec2f ndcToFrameBuffer(Vec2f ndc) const
    {
        return Vec2f(m_Viewport.x + static_cast<float>(ndc.x * m_Viewport.width),
                     m_Viewport.y + static_cast<float>(ndc.y * m_Viewport.height));
    }

private:
    FrameBuffer&          m_FrameBuffer;
    Viewport              m_Viewport;
    RendererScenePassData m_ScenePassData;
};

} // namespace gwars