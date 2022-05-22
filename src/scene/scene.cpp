/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scene.cpp
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

#include "scene/scene.hpp"
#include "ecs/entity_view.hpp"

using namespace gwars;

Entity Scene::createEntity() { return Entity(m_Entities.createEntity(), m_Entities); }

EntityManager& Scene::getEntityManager() { return m_Entities; }

void Scene::onUpdate(float dt) {}

void Scene::render(Renderer& renderer)
{
    renderer.setColor(Color(10, 0, 10, 255));
    renderer.clear();

    const Vec2f origin{0, 0};
    const Vec2f end{0.2, 0};
    for (auto [entity, component] : getView<TransformComponent>(m_Entities))
    {
        Vec2f originResult = component.calculateMatrix() * Vec3f(origin, 1);
        Vec2f endResult    = component.calculateMatrix() * Vec3f(end, 1);

        renderer.setColor(0xFFFF00FF);
        renderer.drawLine(originResult, endResult, 2);
    }
}