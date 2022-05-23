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
        transform.rotation += 0.5 * dt;
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

#include <stdio.h>
void GameLayer::onInit()
{
    m_GameScene.onInit();

    printf("GameLayer::onInit() started\n");

    Entity line = m_GameScene.createEntity();
    line.createComponent<TransformComponent>();
    line.getComponent<TransformComponent>().scale.x = 16;
    line.createComponent<PolygonComponent>(Polygon::createLine());
    line.getComponent<PolygonComponent>().polygon.color = 0xFF0000AA;
    line.getComponent<PolygonComponent>().polygon.thickness = 2;
    line.createComponent<ScriptComponent>(new MovementScript());

    printf("GameLayer::onInit(): line created!\n");

    Entity triangle = m_GameScene.createEntity();
    triangle.createComponent<TransformComponent>(Vec2f(22, -22), 0, Vec2f(10, 10));
    triangle.createComponent<PolygonComponent>(Polygon::createTriangle());
    triangle.getComponent<PolygonComponent>().polygon.color = 0xFF00FF00;
    triangle.getComponent<PolygonComponent>().polygon.thickness = 2;
    triangle.createComponent<ScriptComponent>(new MovementScript());

    Entity quad = m_GameScene.createEntity();
    quad.createComponent<TransformComponent>(Vec2f(-80, 22), 0, Vec2f(10, 5));
    quad.createComponent<PolygonComponent>(Polygon::createQuad());
    quad.getComponent<PolygonComponent>().polygon.color = 0xFFFF00FF;
    quad.getComponent<PolygonComponent>().polygon.thickness = 3;
    quad.createComponent<ScriptComponent>(new MovementScript());

    Entity camera = m_GameScene.createEntity();
    camera.createComponent<TransformComponent>(Vec2f(-100, -100));
    camera.createComponent<CameraComponent>(OrthographicCameraSpecs(1024, 768), true);

    printf("GameLayer::onInit() finished\n");
}

void GameLayer::onUpdate(float dt)
{
    m_GameScene.onUpdate(dt);
}

void GameLayer::onRender(Renderer& renderer) { m_GameScene.render(renderer); }

} // namespace gwars