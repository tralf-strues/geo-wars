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

CollisionEvent::CollisionEvent(Entity firstEntity, Entity secondEntity)
    : firstEntity(firstEntity), secondEntity(secondEntity)
{
}

Scene::Scene(EventDispatcher& eventDispatcher) : m_EventDispatcher(eventDispatcher) {}

Entity Scene::createEntity() { return Entity(m_Entities.createEntity(), m_Entities); }

void Scene::submitToRemoveEntity(Entity entity) { m_EntitiesToRemove.insert(entity); }

bool Scene::isSubmittedToRemove(Entity entity) { return m_EntitiesToRemove.find(entity) != m_EntitiesToRemove.end(); }

EntityManager&   Scene::getEntityManager() { return m_Entities; }
EventDispatcher& Scene::getEventDispatcher() { return m_EventDispatcher; }
Entity           Scene::getMainCamera() { return m_MainCamera; }

bool Scene::isStopped() const { return m_Stopped; }
void Scene::setStropped(bool stopped) { m_Stopped = stopped; }

void Scene::onInit()
{
    m_Entities.onConstruct<ScriptComponent>().addHandler<&Scene::onScriptAdded>(*this);
    m_Entities.onConstruct<CameraComponent>().addHandler<&Scene::onCameraAdded>(*this);

    m_Stopped = false;
}

void Scene::onScriptAdded(const EventComponentConstruct<ScriptComponent>& event)
{
    event.component.nativeScript->onAttach(Entity(event.entityId, m_Entities), m_EventDispatcher);
}

void Scene::onScriptRemoved(const EventComponentRemove<ScriptComponent>& event)
{
    event.component.nativeScript->onDetach(Entity(event.entityId, m_Entities), m_EventDispatcher);
}

void Scene::onCameraAdded(const EventComponentConstruct<CameraComponent>& event)
{
    if (event.component.isMain)
    {
        m_MainCamera = Entity(event.entityId, m_Entities);
    }
}

void Scene::onUpdate(float dt)
{
    /* Running native scripts */
    for (auto [entity, scriptComponent] : getView<ScriptComponent>(m_Entities))
    {
        scriptComponent.nativeScript->onUpdate(dt);
    }

    /* Updating particles */
    for (auto [entity, particleSystemComponent] : getView<ParticleSystemComponent>(m_Entities))
    {
        particleSystemComponent.particleSystem.onUpdate(dt);
    }

    /* Physics simulation TODO: move to a physics scene! */
    for (auto [entity, physicsComponent] : getView<PhysicsComponent>(m_Entities))
    {
        Vec2f acceleration = physicsComponent.force / physicsComponent.mass;
        physicsComponent.velocity += acceleration * dt;

        entity.getComponent<TransformComponent>().translation += physicsComponent.velocity * dt;
    }

    for (auto [entity, boundingSphereComponent] : getView<BoundingSphereComponent>(m_Entities))
    {
        TransformComponent& transform = entity.getComponent<TransformComponent>();

        boundingSphereComponent.wsTranslation = Vec2f(transform.calculateMatrix()
                                                      * Vec3f(boundingSphereComponent.msTranslation, 1));

        boundingSphereComponent.wsRadius = boundingSphereComponent.msRadius
                                           * std::max(transform.scale.x, transform.scale.y);
    }

    for (auto [entity1, boundingSphere1] : getView<BoundingSphereComponent>(m_Entities))
    {
        for (auto [entity2, boundingSphere2] : getView<BoundingSphereComponent>(m_Entities))
        {
            if (/*TODO: add event queue to prevent order dependency*/ !m_Stopped && (entity1 != entity2)
                && !isSubmittedToRemove(entity1) && !isSubmittedToRemove(entity2)
                && boundingSpheresCollide(boundingSphere1, boundingSphere2))
            {
                m_EventDispatcher.fireEvent<CollisionEvent>(entity1, entity2);
            }
        }
    }

    /* Remove submitted entities */
    for (auto entity : m_EntitiesToRemove)
    {
        entity.destroy();
    }

    m_EntitiesToRemove.clear();
}

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
        return;
    }

    renderer.beginScene(mainCameraSpecs, mainCameraViewMatrix);

    renderer.clear(Color(10, 0, 10, 0));

    for (auto [polygon, component] : getView<PolygonComponent>(m_Entities))
    {
        assert(polygon.hasComponent<TransformComponent>());
        renderer.drawPolygon(component.polygon, polygon.getComponent<TransformComponent>().calculateMatrix());
    }

    /* Rendering particles */
    for (auto [entity, particleSystemComponent] : getView<ParticleSystemComponent>(m_Entities))
    {
        particleSystemComponent.particleSystem.onRender(renderer);
    }

    renderer.endScene();
}