#include "physics_system.hpp"
#include "../dispatcher.hpp"
#include "../entity.hpp"
#include "../math/functions.hpp"
#include "physics_component.hpp"
#include <sstream>

PhysicsSystem::PhysicsSystem()
{

    b2Vec2 gravity(0, 0);
    world = new b2World(gravity);

    service::entity::ref().construction<PhysicsComponent>().connect<PhysicsSystem, &PhysicsSystem::createdComponent>(this);
    service::entity::ref().destruction<PhysicsComponent>().connect<PhysicsSystem, &PhysicsSystem::destroyComponent>(this);
    service::dispatcher::ref().sink<Vec2f::RepositionEvent<Entity>>().connect(this);
}

void PhysicsSystem::receive(const WorldRepositionEvent& event)
{
    service::entity::ref().view<PhysicsComponent, Vec2f>().each([](const Entity entity, PhysicsComponent& component, Vec2f& position) {
        component.setPosition(position);
    });
}

void PhysicsSystem::update(unsigned int delta)
{

    float seconds = delta / 1000.0f;
    world->Step(seconds, 12, 5);
    
    service::entity::ref().view<PhysicsComponent, Vec2f>().each([](const Entity entity, PhysicsComponent& component, Vec2f& position) {
        position = component.getPosition();
    });
}

b2World* PhysicsSystem::getWorld()
{
    return world;
}

void PhysicsSystem::createdComponent(EntityRegistry& registry, Entity entity)
{
    auto& position = registry.get<Vec2f>(entity);
    auto& component = registry.get<PhysicsComponent>(entity);

    component.setPosition(position);
}

void PhysicsSystem::destroyComponent(EntityRegistry& registry, Entity entity)
{
    world->DestroyBody(registry.get<PhysicsComponent>(entity).body);
}
