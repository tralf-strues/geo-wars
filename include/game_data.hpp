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
#include "renderer/particle_system.hpp"
#include "scene/scene.hpp"

namespace gwars {

//==================================================================================================
// Game Data
//==================================================================================================
struct GWarsEntityComponent
{
    enum class EntityType
    {
        Invalid = -1,
        Player,
        SpaceshipProjectile,
        Ufo,

        Total
    } entityType;

    GWarsEntityComponent(EntityType entityType = EntityType::Invalid) : entityType(entityType) {}
};

constexpr size_t GWARS_ENTITY_TYPES = static_cast<size_t>(GWarsEntityComponent::EntityType::Total);

using Score = uint64_t;
using Level = uint64_t;

struct ScoreComponent
{
    Score score{0};

    ScoreComponent() = default;
};

struct EnemyLevelComponent
{
    Level level;

    EnemyLevelComponent(Level level = 1) : level(level) {}
};

struct EnemyKilledEvent
{
    Entity enemy;
    Score  killScore{0};

    EnemyKilledEvent() = default;
    EnemyKilledEvent(Entity enemy, Score killScore) : enemy(enemy), killScore(killScore) {}
};

//==================================================================================================
// Constants
//==================================================================================================
extern const Polygon SPACESHIP_MODEL;
extern const Polygon SPACESHIP_PROJECTILE_MODEL;

extern const Vec2f SPACESHIP_SCALE;
extern const Vec2f SPACESHIP_FORWARD;
extern const float SPACESHIP_FORWARD_ENGINE_FORCE;
extern const float SPACESHIP_PERPENDICULAR_ENGINE_FORCE;
extern const float SPACESHIP_FRICTION;
extern const Vec2f SPACESHIP_LEFT_GUN_POSITION;
extern const Vec2f SPACESHIP_RIGHT_GUN_POSITION;
extern const Vec2f SPACESHIP_LEFT_REACTOR_POSITION;
extern const Vec2f SPACESHIP_RIGHT_REACTOR_POSITION;
extern const float SPACESHIP_PROJECTILE_VELOCITY;
extern const float SPACESHIP_RECHARGE_TIME;
extern const Vec2f SPACESHIP_BOUNDING_SPHERE_TRANSLATION;
extern const float SPACESHIP_BOUNDING_SPHERE_RADIUS;
extern const Vec2f SPACESHIP_PROJECTILE_BOUNDING_SPHERE_TRANSLATION;
extern const float SPACESHIP_PROJECTILE_BOUNDING_SPHERE_RADIUS;

extern const float SAFE_SPAWN_RADIUS;

extern const Polygon UFO_MODEL;
extern const Vec2f   UFO_SCALE;
extern const Vec2f   UFO_REACTOR_POSITION0;
extern const Vec2f   UFO_REACTOR_POSITION1;
extern const Vec2f   UFO_REACTOR_POSITION2;
extern const Vec2f   UFO_REACTOR_POSITION3;
extern const float   UFO_VELOCITY_BASE;
extern const float   UFO_VELOCITY_INCREMENT;
extern const float   UFO_VELOCITY_MAX;
extern const Vec2f   UFO_BOUNDING_SPHERE_TRANSLATION;
extern const float   UFO_BOUNDING_SPHERE_RADIUS;

extern const Score UFO_SCORE;

//==================================================================================================
// PlayerControlScript
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
    void emit(Vec2f position);

    Vec2f calculateForward();

    void onKeyPressed(const KeyPressedEvent& event);
    void onKeyReleased(const KeyReleasedEvent& event);
    void onMouseMoved(const MouseMoveEvent& event);
    void onMouseButtonPressed(const MouseButtonPressedEvent& event);
    void onMouseButtonReleased(const MouseButtonReleasedEvent& event);

    void onEnemyKilledEvent(const EnemyKilledEvent& event);

private:
    Entity m_Entity;
    Scene& m_Scene;
    Vec2f  m_EngineForce{0, 0};
    bool   m_Shooting{false};
    float  m_Recharge{0};

    ParticleSystem::ParticleSpecs m_ParticleSpecs;
};

//==================================================================================================
// CollisionHandlerScript
//==================================================================================================
class CollisionHandlerScript : public INativeScript
{
public:
    CollisionHandlerScript(Scene& scene, Entity player);
    virtual ~CollisionHandlerScript() override = default;

    virtual void onAttach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onDetach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onUpdate(float dt) override;

private:
    void onCollisionDetected(CollisionEvent event);

    void onCollisionPlayerUfo(Entity player, Entity ufo);
    void onCollisionProjectileUfo(Entity projectile, Entity ufo);

    typedef void (CollisionHandlerScript::*onCollisionFunctionT)(Entity first, Entity second);
    static const onCollisionFunctionT COLLISION_HANDLERS[GWARS_ENTITY_TYPES][GWARS_ENTITY_TYPES];

private:
    Scene& m_Scene;
    Entity m_Player;
};

//==================================================================================================
// EnemySpawnerScript
//==================================================================================================
class EnemySpawnerScript : public INativeScript
{
public:
    EnemySpawnerScript(Scene& scene, Entity player);
    virtual ~EnemySpawnerScript() override = default;

    virtual void onAttach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onDetach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onUpdate(float dt) override;

private:
    void onEnemyKilledEvent(const EnemyKilledEvent& event);
    void spawn();

private:
    Scene&  m_Scene;
    Entity  m_Player;
    int32_t m_EnemiesLeft{0};
    Level   m_EnemyLevel{1};
};

//==================================================================================================
// EnemyMovementScript
//==================================================================================================
class EnemyMovementScript : public INativeScript
{
public:
    EnemyMovementScript(Entity player) : m_Player(player) {}

    virtual ~EnemyMovementScript() override = default;

    virtual void onAttach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onDetach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onUpdate(float dt) override;

private:
    Vec2f calculateForward();
    void  emit(Vec2f position);

private:
    Entity                        m_Entity;
    Entity                        m_Player;
    ParticleSystem::ParticleSpecs m_ParticleSpecs;
};

//==================================================================================================
// ExplosionScript
//==================================================================================================
class ExplosionScript : public INativeScript
{
public:
    ExplosionScript()                   = default;
    virtual ~ExplosionScript() override = default;

    virtual void onAttach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onDetach(Entity entity, EventDispatcher& eventDispatcher) override;
    virtual void onUpdate(float dt) override;

private:
    void onEnemyKilledEvent(EnemyKilledEvent event);

private:
    Entity m_Entity;
    ParticleSystem::ParticleSpecs m_ParticleSpecs;
};

} // namespace gwars