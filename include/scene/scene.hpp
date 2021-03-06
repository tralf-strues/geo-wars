/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scene.hpp
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

#pragma once

#include "ecs/entity.hpp"
#include "events/event_dispatcher.hpp"
#include "renderer/renderer.hpp"
#include "scene/components.hpp"
#include <set>

namespace gwars {

struct CollisionEvent
{
    Entity firstEntity;
    Entity secondEntity;

    CollisionEvent() = default;
    CollisionEvent(Entity firstEntity, Entity secondEntity);
};

class Scene
{
public:
    Scene(EventDispatcher& eventDispatcher);

    Entity createEntity();
    void   submitToRemoveEntity(Entity entity);

    bool isSubmittedToRemove(Entity entity);

    EntityManager&   getEntityManager();
    EventDispatcher& getEventDispatcher();
    Entity           getMainCamera();

    bool isStopped() const;
    void setStropped(bool stopped);

    void onInit();
    void onUpdate(float dt);
    void render(Renderer& renderer);

private:
    void onScriptAdded(const EventComponentConstruct<ScriptComponent>& event);
    void onScriptRemoved(const EventComponentRemove<ScriptComponent>& event);
    void onCameraAdded(const EventComponentConstruct<CameraComponent>& event);

private:
    EntityManager    m_Entities;
    EventDispatcher& m_EventDispatcher;
    Entity           m_MainCamera;
    std::set<Entity> m_EntitiesToRemove;
    bool             m_Stopped{true};
};

} // namespace gwars