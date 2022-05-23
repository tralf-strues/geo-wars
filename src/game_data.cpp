/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file game_data.cpp
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

#include "game_data.hpp"
#include "assets_management/polygon_loader.hpp"

namespace gwars {

const Polygon PLAYER_SPACESHIP_MODEL = loadPolygon("assets/player_spaceship.txt");

//==================================================================================================
// Game Scripts
//==================================================================================================
const Vec2f PlayerControlScript::FORWARD = Vec2f(0, 1);

void PlayerControlScript::onAttach(Entity entity, EventDispatcher& eventDispatcher)
{
    m_Entity = entity;
    eventDispatcher.getSink<KeyPressedEvent>().addHandler<&PlayerControlScript::onKeyPressed>(*this);
    eventDispatcher.getSink<MouseMoveEvent>().addHandler<&PlayerControlScript::onMouseMoved>(*this);
}

void PlayerControlScript::onDetach(Entity entity, EventDispatcher& eventDispatcher)
{
    eventDispatcher.getSink<KeyPressedEvent>().removeHandler<&PlayerControlScript::onKeyPressed>(*this);
    eventDispatcher.getSink<MouseMoveEvent>().removeHandler<&PlayerControlScript::onMouseMoved>(*this);
}

void PlayerControlScript::onUpdate(float dt)
{
    assert(m_Entity.hasComponent<TransformComponent>());

    TransformComponent& transform = m_Entity.getComponent<TransformComponent>();
    transform.translation += m_Velocity * dt;
    // transform.rotation += 0.5 * dt;
}

void PlayerControlScript::onKeyPressed(const KeyPressedEvent& event)
{
    switch (event.key)
    {
        case Key::Left:
        {
            m_Velocity.x -= 1;
            break;
        }
        case Key::Right:
        {
            m_Velocity.x += 1;
            break;
        }
        case Key::Down:
        {
            m_Velocity.y -= 1;
            break;
        }
        case Key::Up:
        {
            m_Velocity.y += 1;
            break;
        }
        default:
        {
            break;
        }
    }
}

void PlayerControlScript::onMouseMoved(const MouseMoveEvent& event)
{
    TransformComponent& transform = m_Entity.getComponent<TransformComponent>();

    Entity mainCamera = m_Scene.getMainCamera();

    Vec2f world = mainCamera.getComponent<TransformComponent>().calculateMatrix()
                  * mainCamera.getComponent<CameraComponent>().cameraSpecs.calculateInverseProjectionMatrix()
                  * Vec3f(event.x, event.y, 1);
    Vec2f forward = normalize(world - transform.translation);

    if (forward.y == 0)
    {
        transform.rotation = (forward.x >= 0) ? -M_PI / 2 : M_PI / 2;
    }
    else if (forward.y > 0)
    {
        transform.rotation = -atanf(forward.x / forward.y);
    }
    else
    {
        transform.rotation = M_PI - atanf(forward.x / forward.y);
    }
}

} // namespace gwars