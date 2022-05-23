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
#include "utils/random.hpp"

namespace gwars {

//==================================================================================================
// Constants
//==================================================================================================
const Polygon SPACESHIP_MODEL            = loadPolygon("assets/player_spaceship.txt");
const Polygon SPACESHIP_PROJECTILE_MODEL = loadPolygon("assets/player_spaceship_projectile.txt");

const Vec2f SPACESHIP_SCALE                      = Vec2f(15, 15);
const Vec2f SPACESHIP_FORWARD                    = Vec2f(0, 1);
const float SPACESHIP_FORWARD_ENGINE_FORCE       = 100;
const float SPACESHIP_PERPENDICULAR_ENGINE_FORCE = SPACESHIP_FORWARD_ENGINE_FORCE;
const float SPACESHIP_FRICTION                   = 2;
const Vec2f SPACESHIP_LEFT_GUN_POSITION          = Vec2f(-1.15, 1.005);
const Vec2f SPACESHIP_RIGHT_GUN_POSITION         = Vec2f(-SPACESHIP_LEFT_GUN_POSITION.x, SPACESHIP_LEFT_GUN_POSITION.y);
const Vec2f SPACESHIP_LEFT_REACTOR_POSITION      = Vec2f(-0.535, -0.59);
const Vec2f SPACESHIP_RIGHT_REACTOR_POSITION     = Vec2f(-SPACESHIP_LEFT_REACTOR_POSITION.x,
                                                     SPACESHIP_LEFT_REACTOR_POSITION.y);
const float SPACESHIP_PROJECTILE_VELOCITY        = 750;
const float SPACESHIP_RECHARGE_TIME              = 0.15f;
const Vec2f SPACESHIP_BOUNDING_SPHERE_TRANSLATION            = Vec2f(0, 0.7);
const float SPACESHIP_BOUNDING_SPHERE_RADIUS                 = sqrtf(2.2525);
const Vec2f SPACESHIP_PROJECTILE_BOUNDING_SPHERE_TRANSLATION = Vec2f(0, 0);
const float SPACESHIP_PROJECTILE_BOUNDING_SPHERE_RADIUS      = 0.3;

const float SAFE_SPAWN_RADIUS = 300;

const Polygon UFO_MODEL                       = loadPolygon("assets/ufo.txt");
const Vec2f   UFO_SCALE                       = Vec2f(18, 18);
const Vec2f   UFO_REACTOR_POSITION0           = Vec2f(-0.4, -0.5);
const Vec2f   UFO_REACTOR_POSITION1           = Vec2f(0.4, -0.5);
const Vec2f   UFO_REACTOR_POSITION2           = Vec2f(-0.8, -0.5);
const Vec2f   UFO_REACTOR_POSITION3           = Vec2f(0.8, -0.5);
const float   UFO_VELOCITY_BASE               = 50;
const float   UFO_VELOCITY_INCREMENT          = 10;
const float   UFO_VELOCITY_MAX                = 100;
const Vec2f   UFO_BOUNDING_SPHERE_TRANSLATION = Vec2f(0, 0.2);
const float   UFO_BOUNDING_SPHERE_RADIUS      = sqrtf(1.73);

const Score UFO_SCORE = 10;

//==================================================================================================
// PlayerControlScript
//==================================================================================================
void PlayerControlScript::onAttach(Entity entity, EventDispatcher& eventDispatcher)
{
    m_Entity = entity;
    eventDispatcher.getSink<KeyPressedEvent>().addHandler<&PlayerControlScript::onKeyPressed>(*this);
    eventDispatcher.getSink<KeyReleasedEvent>().addHandler<&PlayerControlScript::onKeyReleased>(*this);
    eventDispatcher.getSink<MouseMoveEvent>().addHandler<&PlayerControlScript::onMouseMoved>(*this);
    eventDispatcher.getSink<MouseButtonPressedEvent>().addHandler<&PlayerControlScript::onMouseButtonPressed>(*this);
    eventDispatcher.getSink<MouseButtonReleasedEvent>().addHandler<&PlayerControlScript::onMouseButtonReleased>(*this);
    eventDispatcher.getSink<EnemyKilledEvent>().addHandler<&PlayerControlScript::onEnemyKilledEvent>(*this);

    m_ParticleSpecs.colorBegin = Vec4f(0.05f, 0.2f, 0.8f, 1.0f);
    m_ParticleSpecs.colorEnd = Vec4f(0.2f, 0.6f, 0.8f, 0.0f);

    m_ParticleSpecs.sizeBegin = 0.8f;
    m_ParticleSpecs.sizeEnd = 0.1f;
    m_ParticleSpecs.sizeVariation = 0.3f;

    m_ParticleSpecs.lifetime = 0.2f;
}

void PlayerControlScript::onDetach(Entity, EventDispatcher& eventDispatcher)
{
    /* TODO: Make a convenience function in event dispatcher */
    eventDispatcher.getSink<KeyPressedEvent>().removeHandler<&PlayerControlScript::onKeyPressed>(*this);
    eventDispatcher.getSink<KeyReleasedEvent>().removeHandler<&PlayerControlScript::onKeyReleased>(*this);
    eventDispatcher.getSink<MouseMoveEvent>().removeHandler<&PlayerControlScript::onMouseMoved>(*this);
    eventDispatcher.getSink<MouseButtonPressedEvent>().removeHandler<&PlayerControlScript::onMouseButtonPressed>(*this);
    eventDispatcher.getSink<MouseButtonReleasedEvent>().removeHandler<&PlayerControlScript::onMouseButtonReleased>(
        *this);
    eventDispatcher.getSink<EnemyKilledEvent>().removeHandler<&PlayerControlScript::onEnemyKilledEvent>(*this);
}

void PlayerControlScript::onUpdate(float dt)
{
    assert(m_Entity.hasComponent<TransformComponent>());

    const TransformComponent& transformComponent = m_Entity.getComponent<TransformComponent>();

    PhysicsComponent& physicsComponent = m_Entity.getComponent<PhysicsComponent>();
    Vec2f             forward          = calculateForward();
    Vec2f             right            = perpendicularCounterClockwise(-forward);
    Vec2f             engineForce      = forward * m_EngineForce.y + right * m_EngineForce.x;

    Vec2f frictionForce = (lengthSquare(engineForce) == 0)
                              ? -physicsComponent.mass * SPACESHIP_FRICTION * physicsComponent.velocity
                              : Vec2f(0, 0);

    physicsComponent.force = engineForce + frictionForce;

    m_ParticleSpecs.velocity = -forward * (length(physicsComponent.velocity) + 100.0f);
    m_ParticleSpecs.velocityVariation = 30.0f * right + 100.0f * forward;

    Mat3f transformMatrix = transformComponent.calculateMatrix();
    emit(transformMatrix * Vec3f(SPACESHIP_LEFT_REACTOR_POSITION, 1));
    emit(transformMatrix * Vec3f(SPACESHIP_RIGHT_REACTOR_POSITION, 1));

    if (m_Shooting && m_Recharge <= 0)
    {
        shoot(SPACESHIP_LEFT_GUN_POSITION, forward * SPACESHIP_PROJECTILE_VELOCITY);
        shoot(SPACESHIP_RIGHT_GUN_POSITION, forward * SPACESHIP_PROJECTILE_VELOCITY);
        m_Recharge = SPACESHIP_RECHARGE_TIME;
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
    projectile.createComponent<GWarsEntityComponent>(GWarsEntityComponent::EntityType::SpaceshipProjectile);
    projectile.createComponent<PolygonComponent>(SPACESHIP_PROJECTILE_MODEL);
    projectile.createComponent<PhysicsComponent>(velocity);
    projectile.createComponent<BoundingSphereComponent>(SPACESHIP_PROJECTILE_BOUNDING_SPHERE_RADIUS,
                                                        SPACESHIP_PROJECTILE_BOUNDING_SPHERE_TRANSLATION);
}

void PlayerControlScript::emit(Vec2f position)
{
    m_ParticleSpecs.origin = position;

    for (uint32_t i = 0; i < 10; ++i)
    {
        m_Entity.getComponent<ParticleSystemComponent>().particleSystem.emit(m_ParticleSpecs);
    }
}


Vec2f PlayerControlScript::calculateForward()
{
    return m_Entity.getComponent<TransformComponent>().calculateRotationMatrix() * Vec3f(SPACESHIP_FORWARD, 1);
}

void PlayerControlScript::onKeyPressed(const KeyPressedEvent& event)
{
    switch (event.key)
    {
        case Key::Left:  { m_EngineForce.x -= SPACESHIP_PERPENDICULAR_ENGINE_FORCE; break; }
        case Key::Right: { m_EngineForce.x += SPACESHIP_PERPENDICULAR_ENGINE_FORCE; break; }
        case Key::Down:  { m_EngineForce.y -= SPACESHIP_FORWARD_ENGINE_FORCE; break; }
        case Key::Up:    { m_EngineForce.y += SPACESHIP_FORWARD_ENGINE_FORCE; break; }
        default:         { break; }
    }
}

void PlayerControlScript::onKeyReleased(const KeyReleasedEvent& event)
{
    switch (event.key)
    {
        case Key::Left:  { m_EngineForce.x += SPACESHIP_PERPENDICULAR_ENGINE_FORCE; break; }
        case Key::Right: { m_EngineForce.x -= SPACESHIP_PERPENDICULAR_ENGINE_FORCE; break; }
        case Key::Down:  { m_EngineForce.y += SPACESHIP_FORWARD_ENGINE_FORCE; break; }
        case Key::Up:    { m_EngineForce.y -= SPACESHIP_FORWARD_ENGINE_FORCE; break; }
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

void PlayerControlScript::onMouseButtonPressed(const MouseButtonPressedEvent&)
{
    m_Shooting = true;
}

void PlayerControlScript::onMouseButtonReleased(const MouseButtonReleasedEvent&)
{
    m_Shooting = false;
}

void PlayerControlScript::onEnemyKilledEvent(const EnemyKilledEvent& event)
{
    m_Entity.getComponent<ScoreComponent>().score += event.killScore;
    printf("Player score: %llu\n", m_Entity.getComponent<ScoreComponent>().score);
}

//==================================================================================================
// CollisionHandlerScript
//==================================================================================================
const CollisionHandlerScript::onCollisionFunctionT CollisionHandlerScript::COLLISION_HANDLERS[GWARS_ENTITY_TYPES][GWARS_ENTITY_TYPES] = {
    {                                      nullptr,                                           nullptr,     &CollisionHandlerScript::onCollisionPlayerUfo},
    {                                      nullptr,                                           nullptr, &CollisionHandlerScript::onCollisionProjectileUfo},
    {&CollisionHandlerScript::onCollisionPlayerUfo, &CollisionHandlerScript::onCollisionProjectileUfo,                                           nullptr}
};

CollisionHandlerScript::CollisionHandlerScript(Scene& scene, Entity player) : m_Scene(scene), m_Player(player) {}

void CollisionHandlerScript::onAttach(Entity /*entity*/, EventDispatcher& eventDispatcher)
{
    eventDispatcher.getSink<CollisionEvent>().addHandler<&CollisionHandlerScript::onCollisionDetected>(*this);
}

void CollisionHandlerScript::onDetach(Entity, EventDispatcher& eventDispatcher)
{
    eventDispatcher.getSink<CollisionEvent>().removeHandler<&CollisionHandlerScript::onCollisionDetected>(*this);
}

void CollisionHandlerScript::onUpdate(float) {}

void CollisionHandlerScript::onCollisionDetected(CollisionEvent event)
{
    if (!event.firstEntity.hasComponent<GWarsEntityComponent>()
        || !event.secondEntity.hasComponent<GWarsEntityComponent>())
    {
        return;
    }

    GWarsEntityComponent::EntityType firstType  = event.firstEntity.getComponent<GWarsEntityComponent>().entityType;
    GWarsEntityComponent::EntityType secondType = event.secondEntity.getComponent<GWarsEntityComponent>().entityType;

    if (firstType > secondType)
    {
        Entity tmpEntity   = event.firstEntity;
        event.firstEntity  = event.secondEntity;
        event.secondEntity = tmpEntity;

        GWarsEntityComponent::EntityType tmpType = firstType;
        firstType                                = secondType;
        secondType                               = tmpType;
    }

    onCollisionFunctionT handler = COLLISION_HANDLERS[static_cast<size_t>(firstType)][static_cast<size_t>(secondType)];

    if (handler != nullptr)
    {
        (this->*handler)(event.firstEntity, event.secondEntity);
    }
}

void CollisionHandlerScript::onCollisionPlayerUfo(Entity player, Entity /* ufo */)
{
    printf("GAME OVER!\n");
    printf("Score: %llu\n", player.getComponent<ScoreComponent>().score);
    m_Scene.setStropped(true);
}

void CollisionHandlerScript::onCollisionProjectileUfo(Entity projectile, Entity ufo)
{
    if (!m_Scene.isSubmittedToRemove(projectile) && !m_Scene.isSubmittedToRemove(ufo))
    {
        Level level = ufo.getComponent<EnemyLevelComponent>().level;
        m_Scene.getEventDispatcher().getSink<EnemyKilledEvent>().fireEvent(EnemyKilledEvent(ufo, UFO_SCORE * level));
        m_Scene.submitToRemoveEntity(projectile);
        m_Scene.submitToRemoveEntity(ufo);
    }
}

//==================================================================================================
// EnemySpawnerScript
//==================================================================================================
EnemySpawnerScript::EnemySpawnerScript(Scene& scene, Entity player) : m_Scene(scene), m_Player(player) {}

void EnemySpawnerScript::onAttach(Entity, EventDispatcher& eventDispatcher)
{
    eventDispatcher.getSink<EnemyKilledEvent>().addHandler<&EnemySpawnerScript::onEnemyKilledEvent>(*this);
}

void EnemySpawnerScript::onDetach(Entity, EventDispatcher& eventDispatcher)
{
    eventDispatcher.getSink<EnemyKilledEvent>().removeHandler<&EnemySpawnerScript::onEnemyKilledEvent>(*this);
}

void EnemySpawnerScript::onUpdate(float)
{
    if (m_EnemiesLeft == 0)
    {
        spawn();
        ++m_EnemyLevel;
    }
}

void EnemySpawnerScript::onEnemyKilledEvent(const EnemyKilledEvent&)
{
    --m_EnemiesLeft;
}

void EnemySpawnerScript::spawn()
{
    BoundingSphereComponent safeSpawnSphere = m_Player.getComponent<BoundingSphereComponent>();
    safeSpawnSphere.wsRadius += SAFE_SPAWN_RADIUS;

    int32_t toSpawn = RandomNumberGenerator::randomInRange(2, 7);
    for (int32_t i = 0; i < toSpawn; ++i)
    {
        BoundingSphereComponent ufoSphere;
        Vec2f                   translation;
        do
        {
            translation = Vec2f(RandomNumberGenerator::randomInRange(-500, 500),
                                RandomNumberGenerator::randomInRange(-300, 300));

            ufoSphere.wsTranslation = UFO_BOUNDING_SPHERE_TRANSLATION * UFO_SCALE + translation;
            ufoSphere.wsRadius      = UFO_BOUNDING_SPHERE_RADIUS * std::max(UFO_SCALE.x, UFO_SCALE.y);
        } while (boundingSpheresCollide(ufoSphere, safeSpawnSphere));

        Entity ufo = m_Scene.createEntity();
        ufo.createComponent<TransformComponent>(translation);
        ufo.getComponent<TransformComponent>().scale = UFO_SCALE;
        ufo.createComponent<GWarsEntityComponent>(GWarsEntityComponent::EntityType::Ufo);
        ufo.createComponent<PolygonComponent>(UFO_MODEL);
        ufo.createComponent<PhysicsComponent>();
        ufo.createComponent<ParticleSystemComponent>(2048, loadPolygon("assets/fire_particle.txt"));
        ufo.createComponent<EnemyLevelComponent>(m_EnemyLevel);
        ufo.createComponent<BoundingSphereComponent>(UFO_BOUNDING_SPHERE_RADIUS, UFO_BOUNDING_SPHERE_TRANSLATION);
        ufo.createComponent<ScriptComponent>(new EnemyMovementScript(m_Player));

        ++m_EnemiesLeft;
    }
}

//==================================================================================================
// EnemyMovementScript
//==================================================================================================
void EnemyMovementScript::onAttach(Entity entity, EventDispatcher& /*eventDispatcher*/)
{
    m_Entity = entity;

    m_ParticleSpecs.colorBegin = Vec4f(0.4f, 0.8f, 0.2f, 1.0f);
    m_ParticleSpecs.colorEnd = Vec4f(0.4f, 0.6f, 0.4f, 0.0f);

    m_ParticleSpecs.sizeBegin = 0.5f;
    m_ParticleSpecs.sizeEnd = 0.1f;
    m_ParticleSpecs.sizeVariation = 0.2f;

    m_ParticleSpecs.lifetime = 0.1f;
}

void EnemyMovementScript::onDetach(Entity /*entity*/, EventDispatcher& /*eventDispatcher*/) {}

void EnemyMovementScript::onUpdate(float /*dt*/)
{
    assert(m_Entity.hasComponent<TransformComponent>());

    const TransformComponent& transformComponent = m_Entity.getComponent<TransformComponent>();
    PhysicsComponent&         physicsComponent   = m_Entity.getComponent<PhysicsComponent>();
    Vec2f                     forward            = calculateForward();

    Level level               = m_Entity.getComponent<EnemyLevelComponent>().level;
    physicsComponent.velocity = forward
                                * std::max((UFO_VELOCITY_BASE + (level - 1) * UFO_VELOCITY_INCREMENT),
                                           UFO_VELOCITY_MAX);

    m_ParticleSpecs.velocity          = Vec2f(0, -150);
    m_ParticleSpecs.velocityVariation = Vec2f(150, 50);

    Mat3f transformMatrix = transformComponent.calculateMatrix();
    emit(transformMatrix * Vec3f(UFO_REACTOR_POSITION0, 1));
    emit(transformMatrix * Vec3f(UFO_REACTOR_POSITION1, 1));
    emit(transformMatrix * Vec3f(UFO_REACTOR_POSITION2, 1));
    emit(transformMatrix * Vec3f(UFO_REACTOR_POSITION3, 1));
}

Vec2f EnemyMovementScript::calculateForward()
{
    return normalize(m_Player.getComponent<TransformComponent>().translation
                     - m_Entity.getComponent<TransformComponent>().translation);
}

void EnemyMovementScript::emit(Vec2f position)
{
    m_ParticleSpecs.origin = position;

    for (uint32_t i = 0; i < 7; ++i)
    {
        m_Entity.getComponent<ParticleSystemComponent>().particleSystem.emit(m_ParticleSpecs);
    }
}

//==================================================================================================
// ExplosionScript
//==================================================================================================
void ExplosionScript::onAttach(Entity entity, EventDispatcher& eventDispatcher)
{
    m_Entity = entity;
    eventDispatcher.getSink<EnemyKilledEvent>().addHandler<&ExplosionScript::onEnemyKilledEvent>(*this);

    m_ParticleSpecs.colorBegin = Vec4f(0.9f, 0.8f, 0.1f, 1.0f);
    m_ParticleSpecs.colorEnd   = Vec4f(1.0f, 0.4f, 0.1f, 0.0f);

    m_ParticleSpecs.sizeBegin     = 0.1f;
    m_ParticleSpecs.sizeEnd       = 0.3f;
    m_ParticleSpecs.sizeVariation = 0.05f;

    m_ParticleSpecs.velocityVariation = Vec2f(1000, 1000);

    m_ParticleSpecs.lifetime = 0.4f;
}

void ExplosionScript::onDetach(Entity /*entity*/, EventDispatcher& eventDispatcher)
{
    eventDispatcher.getSink<EnemyKilledEvent>().removeHandler<&ExplosionScript::onEnemyKilledEvent>(*this);
}

void ExplosionScript::onUpdate(float /*dt*/) {}

void ExplosionScript::onEnemyKilledEvent(EnemyKilledEvent event)
{
    assert(m_Entity.hasComponent<ParticleSystemComponent>());

    const TransformComponent& transform = event.enemy.getComponent<TransformComponent>();

    Vec2f translationRange(5, 5);
    for (uint32_t i = 0; i < 512; ++i)
    {
        Vec2f randomDirection{RandomNumberGenerator::randomNormalized(), RandomNumberGenerator::randomNormalized()};

        m_ParticleSpecs.origin = transform.translation
                                 + translationRange * (2 * RandomNumberGenerator::randomNormalized() - 1);
        m_ParticleSpecs.velocity = 200.0f * randomDirection;

        m_Entity.getComponent<ParticleSystemComponent>().particleSystem.emit(m_ParticleSpecs);
    }
}

} // namespace gwars