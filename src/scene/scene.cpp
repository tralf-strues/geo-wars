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
#include <stdio.h>

using namespace gwars;

Scene::Scene(EventDispatcher& eventDispatcher) : m_EventDispatcher(eventDispatcher) {}

Entity Scene::createEntity() { return Entity(m_Entities.createEntity(), m_Entities); }

EntityManager& Scene::getEntityManager() { return m_Entities; }

void Scene::onUpdate(float dt) {}

void Scene::render(Renderer& renderer)
{
    /* Finding main camera */
    bool                    mainCameraFound{false};
    OrthographicCameraSpecs mainCameraSpecs;
    Mat3f                   mainCameraViewMatrix;
    for (auto [camera, component] : getView<CameraComponent>(m_Entities))
    {
        assert(camera.hasComponent<TransformComponent>());

        if (!component.isMain)
        {
            continue;
        }

        if (mainCameraFound)
        {
            printf("Second main camera has been detected! Using the first detected camera!\n");
        }
        else
        {
            mainCameraFound      = true;
            mainCameraSpecs      = component.cameraSpecs;
            mainCameraViewMatrix = camera.getComponent<TransformComponent>().calculateInverseMatrix();
        }
    }

    if (!mainCameraFound)
    {
        printf("No main camera detected!\n");
        return;
    }

    renderer.beginScene(mainCameraSpecs, mainCameraViewMatrix);

    renderer.clear(Color(10, 0, 10, 255));

    for (auto [line, component] : getView<LineComponent>(m_Entities))
    {
        assert(line.hasComponent<TransformComponent>());

        Line worldSpaceLine = component.line;
        worldSpaceLine.from = line.getComponent<TransformComponent>().calculateMatrix() * Vec3f(worldSpaceLine.from);
        worldSpaceLine.to   = line.getComponent<TransformComponent>().calculateMatrix() * Vec3f(worldSpaceLine.to);
        renderer.drawLine(worldSpaceLine);
    }

    renderer.endScene();
}