/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file game_data.hpp
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

#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "renderer/draw_primitives.hpp"
#include "scene/scene.hpp"

namespace gwars {

extern const Polygon SPACESHIP_MODEL;
extern const Polygon SPACESHIP_PROJECTILE_MODEL;

static const Vec2f SPACESHIP_FORWARD;
static const float SPACESHIP_FORWARD_ENGINE_FORCE;
static const float SPACESHIP_PERPENDICULAR_ENGINE_FORCE;
static const float SPACESHIP_FRICTION;
static const Vec2f SPACESHIP_LEFT_GUN_POSITION;
static const Vec2f SPACESHIP_RIGHT_GUN_POSITION;
static const float SPACESHIP_PROJECTILE_VELOCITY;
static const float SPACESHIP_RECHARGE_TIME;
static const Vec2f SPACESHIP_BOUNDING_SPHERE_TRANSLATION;
static const float SPACESHIP_BOUNDING_SPHERE_RADIUS;

//==================================================================================================
// Game Scripts
//==================================================================================================
class PlayerControlScript : public INativeScript
{
public:
    PlayerControlScript(Scene& scene) : m_Scene(scene) {}

    virtual ~PlayerControlScript() override = default;

    virtual void onAttach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onDetach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onUpdate(float dt) override;

private:
    void shoot(Vec2f position, Vec2f velocity);

    Vec2f calculateForward();

    void onKeyPressed(const KeyPressedEvent& event);
    void onKeyReleased(const KeyReleasedEvent& event);
    void onMouseMoved(const MouseMoveEvent& event);
    void onMouseButtonPressed(const MouseButtonPressedEvent& event);
    void onMouseButtonReleased(const MouseButtonReleasedEvent& event);

private:
    Entity m_Entity;
    Scene& m_Scene;
    Vec2f  m_EngineForce{0, 0};
    bool   m_Shooting{false};
    float  m_Recharge{0};
};

} // namespace gwars