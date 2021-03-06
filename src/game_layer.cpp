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
#include "assets_management/polygon_loader.hpp"
#include "game_data.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

namespace gwars {

GameLayer::GameLayer(EventDispatcher& eventDispatcher) : m_GameScene(eventDispatcher) {}

bool GameLayer::isStopped() const { return m_Stopped; }
void GameLayer::setStopped(bool stopped) { m_Stopped = stopped; }

void GameLayer::onInit()
{
    m_GameScene.onInit();

    Entity player = m_GameScene.createEntity();
    player.createComponent<TransformComponent>(Vec2f(50, 50));
    player.getComponent<TransformComponent>().scale = SPACESHIP_SCALE;
    player.createComponent<GWarsEntityComponent>(GWarsEntityComponent::EntityType::Player);
    player.createComponent<ScoreComponent>();
    player.createComponent<PolygonComponent>(SPACESHIP_MODEL);
    player.createComponent<ParticleSystemComponent>(2048, loadPolygon("assets/fire_particle.txt"));
    player.createComponent<ScriptComponent>(new PlayerControlScript(m_GameScene));
    player.createComponent<PhysicsComponent>();
    player.createComponent<BoundingSphereComponent>(SPACESHIP_BOUNDING_SPHERE_RADIUS,
                                                    SPACESHIP_BOUNDING_SPHERE_TRANSLATION);

    Entity collisionHandler = m_GameScene.createEntity();
    collisionHandler.createComponent<ScriptComponent>(new CollisionHandlerScript(m_GameScene, player));

    Entity enemySpawner = m_GameScene.createEntity();
    enemySpawner.createComponent<ScriptComponent>(new EnemySpawnerScript(m_GameScene, player));

    Entity explosionHandler = m_GameScene.createEntity();
    explosionHandler.createComponent<ScriptComponent>(new ExplosionScript());
    explosionHandler.createComponent<ParticleSystemComponent>(512, loadPolygon("assets/fire_particle.txt"));

    Entity camera = m_GameScene.createEntity();
    camera.createComponent<TransformComponent>();
    camera.createComponent<CameraComponent>(OrthographicCameraSpecs(1024, 768), true);
}

void GameLayer::onUpdate(float dt)
{
    if (m_GameScene.isStopped())
    {
        setStopped(true);
    }

    m_GameScene.onUpdate(dt);
}

void GameLayer::onRender(Renderer& renderer) { m_GameScene.render(renderer); }

} // namespace gwars