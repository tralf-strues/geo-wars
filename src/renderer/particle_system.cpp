/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file particle_system.cpp
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

#include "renderer/particle_system.hpp"
#include "utils/random.hpp"

namespace gwars {

ParticleSystem::ParticleSystem(size_t poolSize, const Polygon& particlePolygon) : m_ParticlePolygon(particlePolygon)
{
    m_Particles.resize(poolSize);
}

#include <stdio.h>
void ParticleSystem::onUpdate(float dt)
{
    for (auto& particle : m_Particles)
    {
        if (!particle.active)
        {
            continue;
        }

        if (particle.timeRemaining <= 0.0f)
        {
            particle.active = false;
            continue;
        }

        particle.translation += particle.velocity * dt;
        particle.rotation += PARTICLE_ROTATION_RATE * dt;
        particle.timeRemaining -= dt;
    }
}

void ParticleSystem::onRender(Renderer& renderer)
{
    for (auto& particle : m_Particles)
    {
        if (!particle.active)
        {
            continue;
        }

        float lifetimePercentage = particle.timeRemaining / particle.lifetime;
        Vec4f color              = lerp(particle.colorEnd, particle.colorBegin, lifetimePercentage);
        float size               = lerp(particle.sizeEnd, particle.sizeBegin, lifetimePercentage);

        m_ParticlePolygon.color = Color(color);
        Mat3f transform         = translationMatrix(particle.translation) * rotationMatrix(particle.rotation)
                          * scaleMatrix(Vec2f(size, size));

        renderer.drawPolygon(m_ParticlePolygon, transform);
    }
}

void ParticleSystem::emit(const ParticleSpecs& particleSpecs)
{
    Particle& particle = m_Particles[m_NextParticle];
    particle.active    = true;

    particle.translation = particleSpecs.origin;
    particle.rotation    = RandomNumberGenerator::randomNormalized() * M_PI;

    particle.velocity = particleSpecs.velocity;
    particle.velocity.x += (RandomNumberGenerator::randomNormalized() - 0.5f) * particleSpecs.velocityVariation.x;
    particle.velocity.y += (RandomNumberGenerator::randomNormalized() - 0.5f) * particleSpecs.velocityVariation.y;

    particle.colorBegin = particleSpecs.colorBegin;
    particle.colorEnd   = particleSpecs.colorEnd;

    particle.lifetime      = particleSpecs.lifetime;
    particle.timeRemaining = particleSpecs.lifetime;
    particle.sizeBegin     = particleSpecs.sizeBegin;
    particle.sizeEnd       = particleSpecs.sizeEnd;
    particle.sizeBegin += particleSpecs.sizeVariation * (RandomNumberGenerator::randomNormalized() - 0.5f);

    m_NextParticle = (m_NextParticle + 1) % m_Particles.size();
}

} // namespace gwars