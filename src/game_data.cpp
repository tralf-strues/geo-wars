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

const Polygon PLAYER_SPACESHIP_MODEL            = loadPolygon("assets/player_spaceship.txt");
const Polygon PLAYER_SPACESHIP_PROJECTILE_MODEL = loadPolygon("assets/player_spaceship_projectile.txt");

//==================================================================================================
// Game Scripts
//==================================================================================================
const Vec2f PlayerControlScript::FORWARD                    = Vec2f(0, 1);
const float PlayerControlScript::FORWARD_ENGINE_FORCE       = 75;
const float PlayerControlScript::PERPENDICULAR_ENGINE_FORCE = PlayerControlScript::FORWARD_ENGINE_FORCE;
const float PlayerControlScript::FRICTION                   = 1;
const Vec2f PlayerControlScript::LEFT_GUN_POSITION          = Vec2f(-1.15, 1.005);
const Vec2f PlayerControlScript::RIGHT_GUN_POSITION         = Vec2f(-PlayerControlScript::LEFT_GUN_POSITION.x,
                                                            PlayerControlScript::LEFT_GUN_POSITION.y);
const float PlayerControlScript::PROJECTILE_VELOCITY        = 250;
const float PlayerControlScript::RECHARGE_TIME              = 0.3f;

void PlayerControlScript::onAttach(Entity entity, EventDispatcher& eventDispatcher)
{
    m_Entity = entity;
    eventDispatcher.getSink<KeyPressedEvent>().addHandler<&PlayerControlScript::onKeyPressed>(*this);
    eventDispatcher.getSink<KeyReleasedEvent>().addHandler<&PlayerControlScript::onKeyReleased>(*this);
    eventDispatcher.getSink<MouseMoveEvent>().addHandler<&PlayerControlScript::onMouseMoved>(*this);
    eventDispatcher.getSink<MouseButtonPressedEvent>().addHandler<&PlayerControlScript::onMouseButtonPressed>(*this);
    eventDispatcher.getSink<MouseButtonReleasedEvent>().addHandler<&PlayerControlScript::onMouseButtonReleased>(*this);
}

void PlayerControlScript::onDetach(Entity entity, EventDispatcher& eventDispatcher)
{
    eventDispatcher.getSink<KeyPressedEvent>().removeHandler<&PlayerControlScript::onKeyPressed>(*this);
    eventDispatcher.getSink<KeyReleasedEvent>().removeHandler<&PlayerControlScript::onKeyReleased>(*this);
    eventDispatcher.getSink<MouseMoveEvent>().removeHandler<&PlayerControlScript::onMouseMoved>(*this);
    eventDispatcher.getSink<MouseButtonPressedEvent>().removeHandler<&PlayerControlScript::onMouseButtonPressed>(*this);
    eventDispatcher.getSink<MouseButtonReleasedEvent>().removeHandler<&PlayerControlScript::onMouseButtonReleased>(
        *this);
}

void PlayerControlScript::onUpdate(float dt)
{
    assert(m_Entity.hasComponent<TransformComponent>());

    PhysicsComponent& physicsComponent = m_Entity.getComponent<PhysicsComponent>();
    Vec2f             forward          = calculateForward();
    Vec2f             right            = perpendicularCounterClockwise(forward);
    Vec2f             engineForce      = forward * m_EngineForce.y + right * m_EngineForce.x;

    Vec2f frictionForce = (lengthSquare(engineForce) == 0)
                              ? -physicsComponent.mass * FRICTION * physicsComponent.velocity
                              : Vec2f(0, 0);

    physicsComponent.force = engineForce + frictionForce;

    if (m_Shooting && m_Recharge <= 0)
    {
        shoot(LEFT_GUN_POSITION, forward * PROJECTILE_VELOCITY);
        shoot(RIGHT_GUN_POSITION, forward * PROJECTILE_VELOCITY);
        m_Recharge = RECHARGE_TIME;
    }
    else
    {
        m_Recharge -= dt;
    }
}

void PlayerControlScript::shoot(Vec2f position, Vec2f velocity)
{
    TransformComponent transform{m_Entity.getComponent<TransformComponent>()};
    transform.translation = transform.calculateMatrix() * Vec3f(position, 1);

    Entity projectile = m_Scene.createEntity();
    projectile.createComponent<TransformComponent>(transform);
    projectile.createComponent<PolygonComponent>(PLAYER_SPACESHIP_PROJECTILE_MODEL);
    projectile.createComponent<PhysicsComponent>(velocity);
}

Vec2f PlayerControlScript::calculateForward()
{
    return m_Entity.getComponent<TransformComponent>().calculateRotationMatrix() * Vec3f(FORWARD, 1);
}

void PlayerControlScript::onKeyPressed(const KeyPressedEvent& event)
{
    switch (event.key)
    {
        case Key::Left:  { m_EngineForce.x -= PERPENDICULAR_ENGINE_FORCE; break; }
        case Key::Right: { m_EngineForce.x += PERPENDICULAR_ENGINE_FORCE; break; }
        case Key::Down:  { m_EngineForce.y -= FORWARD_ENGINE_FORCE; break; }
        case Key::Up:    { m_EngineForce.y += FORWARD_ENGINE_FORCE; break; }
        default:         { break; }
    }
}

void PlayerControlScript::onKeyReleased(const KeyReleasedEvent& event)
{
    switch (event.key)
    {
        case Key::Left:  { m_EngineForce.x += PERPENDICULAR_ENGINE_FORCE; break; }
        case Key::Right: { m_EngineForce.x -= PERPENDICULAR_ENGINE_FORCE; break; }
        case Key::Down:  { m_EngineForce.y += FORWARD_ENGINE_FORCE; break; }
        case Key::Up:    { m_EngineForce.y -= FORWARD_ENGINE_FORCE; break; }
        default:         { break; }
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

void PlayerControlScript::onMouseButtonPressed(const MouseButtonPressedEvent& event)
{
    m_Shooting = true;
}

void PlayerControlScript::onMouseButtonReleased(const MouseButtonReleasedEvent& event)
{
    m_Shooting = false;
}

} // namespace gwars