/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file particle_system.hpp
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

#include "renderer/renderer.hpp"

namespace gwars {

class ParticleSystem
{
public:
    struct ParticleSpecs
    {
        Vec2f origin;
        Vec2f velocity;
        Vec2f velocityVariation;

        Vec4f colorBegin;
        Vec4f colorEnd;

        float sizeBegin{0.0f};
        float sizeEnd{1.0f};
        float sizeVariation{0.0f};

        float lifetime{1.0f};
    };

    ParticleSystem(size_t poolSize, const Polygon& particlePolygon);

    void onUpdate(float dt);
    void onRender(Renderer& renderer);

    void emit(const ParticleSpecs& particleSpecs);

private:
    struct Particle
    {
        Vec2f translation;
        Vec2f velocity;
        float rotation{0.0f};
        Vec4f colorBegin;
        Vec4f colorEnd;

        float sizeBegin{0.0f};
        float sizeEnd{1.0f};

        float lifetime{1.0f};
        float timeRemaining{0.0f};

        bool active{false};
    };

    static constexpr float PARTICLE_ROTATION_RATE = 0.02f;

    Polygon               m_ParticlePolygon;
    std::vector<Particle> m_Particles;
    uint32_t              m_NextParticle{0};

};

} // namespace gwars