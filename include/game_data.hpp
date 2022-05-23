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

extern const Polygon PLAYER_SPACESHIP_MODEL;

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
    static const Vec2f FORWARD;
    static const float FORWARD_ENGINE_FORCE;
    static const float PERPENDICULAR_ENGINE_FORCE;
    static const float FRICTION;

    Vec2f calculateForward();

    void onKeyPressed(const KeyPressedEvent& event);
    void onKeyReleased(const KeyReleasedEvent& event);
    void onMouseMoved(const MouseMoveEvent& event);

private:
    Entity m_Entity;
    Scene& m_Scene;
    Vec2f  m_EngineForce{0, 0};
};

} // namespace gwars