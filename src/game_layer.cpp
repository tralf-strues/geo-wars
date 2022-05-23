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
#include "game_data.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

namespace gwars {

GameLayer::GameLayer(EventDispatcher& eventDispatcher) : m_GameScene(eventDispatcher) {}

#include <stdio.h>
void GameLayer::onInit()
{
    m_GameScene.onInit();

    printf("GameLayer::onInit() started\n");

    Entity player = m_GameScene.createEntity();
    player.createComponent<TransformComponent>(Vec2f(50, 50));
    player.getComponent<TransformComponent>().scale = Vec2f(15, 15);
    player.createComponent<PolygonComponent>(PLAYER_SPACESHIP_MODEL);
    player.createComponent<ScriptComponent>(new PlayerControlScript(m_GameScene));
    player.createComponent<PhysicsComponent>();

    Entity camera = m_GameScene.createEntity();
    camera.createComponent<TransformComponent>();
    camera.createComponent<CameraComponent>(OrthographicCameraSpecs(1024, 768), true);

    printf("GameLayer::onInit() finished\n");
}

void GameLayer::onUpdate(float dt)
{
    m_GameScene.onUpdate(dt);
}

void GameLayer::onRender(Renderer& renderer) { m_GameScene.render(renderer); }

} // namespace gwars