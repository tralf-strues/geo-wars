/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file color.hpp
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

#include <stdint.h>

namespace gwars {

class Color
{
public:
    Color() = default;
    Color(uint32_t color) : m_Color(color) {}
    Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 0xFF) : m_Color((a << 24u) + (b << 16u) + (g << 8u) + r) {}

    inline uint32_t getR() const { return getByte(0); }
    inline uint32_t getG() const { return getByte(1); }
    inline uint32_t getB() const { return getByte(2); }
    inline uint32_t getA() const { return getByte(3); }

    inline void setR(uint32_t r) { m_Color = (m_Color & ~(0xFF)) | (r); }
    inline void setG(uint32_t g) { m_Color = (m_Color & ~(0xFF << 8u)) | (g << 8u); }
    inline void setB(uint32_t b) { m_Color = (m_Color & ~(0xFF << 16u)) | (b << 16u); }
    inline void setA(uint32_t a) { m_Color = (m_Color & ~(0xFF << 24u)) | (a << 24u); }

    inline operator uint32_t() const { return m_Color; }

private:
    inline uint32_t getByte(uint8_t byteNumber) const
    {
        return (m_Color & (0xFFu << (8u * byteNumber))) >> (8u * byteNumber);
    }

private:
    uint32_t m_Color{0}; // ABGR
};

} // namespace gwars