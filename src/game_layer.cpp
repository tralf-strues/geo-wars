/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file game_layer.cpp
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

#include "game_layer.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

namespace gwars {

Entity g_Line;

GameLayer::GameLayer(EventDispatcher& eventDispatcher) : m_GameScene(eventDispatcher) {}

class MovementScript : public INativeScript
{
public:
    MovementScript() = default;

    virtual ~MovementScript() override = default;

    virtual void onAttach(Entity entity, EventDispatcher& eventDispatcher) override
    {
        m_Entity = entity;
        eventDispatcher.getSink<KeyPressedEvent>().addHandler<&MovementScript::onKeyPressed>(*this);
    }

    virtual void onDetach(Entity entity, EventDispatcher& eventDispatcher) override
    {
        eventDispatcher.getSink<KeyPressedEvent>().removeHandler<&MovementScript::onKeyPressed>(*this);
    }

    virtual void onUpdate(float dt) override
    {
        assert(m_Entity.hasComponent<TransformComponent>());

        TransformComponent& transform = m_Entity.getComponent<TransformComponent>();
        transform.translation += m_Velocity * dt;
    }

private:
    void onKeyPressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case Key::Left:  { m_Velocity.x -= 1; break; }
            case Key::Right: { m_Velocity.x += 1; break; }
            case Key::Down:  { m_Velocity.y -= 1; break; }
            case Key::Up:    { m_Velocity.y += 1; break; }
            default: { break; }
        }
    }

private:
    Entity m_Entity;
    Vec2f m_Velocity{0, 0};
};

void GameLayer::onInit()
{
    m_GameScene.onInit();

    g_Line = m_GameScene.createEntity();
    g_Line.createComponent<TransformComponent>();
    g_Line.getComponent<TransformComponent>().scale.x = 16;
    g_Line.createComponent<LineComponent>();
    g_Line.createComponent<ScriptComponent>(new MovementScript());

    Entity camera = m_GameScene.createEntity();
    camera.createComponent<TransformComponent>(Vec2f(-100, -100));
    camera.createComponent<CameraComponent>(OrthographicCameraSpecs(1024, 768), true);
}

void GameLayer::onUpdate(float dt)
{
    m_GameScene.onUpdate(dt);
    g_Line.getComponent<TransformComponent>().rotation += dt;
}

void GameLayer::onRender(Renderer& renderer) { m_GameScene.render(renderer); }

} // namespace gwars