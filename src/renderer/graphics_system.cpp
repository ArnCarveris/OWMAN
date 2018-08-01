#include "graphics_system.hpp"
#include "sprite_status.hpp"
#include "color.hpp"
#include "../resource_manager/resource_manager.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../entity.hpp"
#include "../util/time_conversions.hpp"

GraphicsSystem::GraphicsSystem(GraphicsSystem&& rhs) :
    camera(std::move(rhs.camera))
{
    auto& sprite_status_destruction = service::entity::ref().destruction<SpriteStatus>();

    sprite_status_destruction.disconnect<GraphicsSystem, &GraphicsSystem::destroyComponent>(&rhs);
    sprite_status_destruction.connect<GraphicsSystem, &GraphicsSystem::destroyComponent>(this);
}

void GraphicsSystem::setFullScreen(bool b)
{
    service::renderer::ref().setFullScreen(b);
}

void GraphicsSystem::update(unsigned int delta)
{

    float deltaSeconds = ticksToSeconds(delta);

    for (auto& it : service::entity::ref().view<SpriteStatus>(entt::raw_t{}))
	{
		it.update(deltaSeconds);
	}

    service::entity::ref().sort<SpriteStatus>
    (
        [](const SpriteStatus& gc1, const SpriteStatus& gc2) -> bool
        {
            return gc1.getPriority() < gc2.getPriority();
        }
    );
    service::entity::ref().sort<Vec2f, SpriteStatus>();
}

void GraphicsSystem::draw()
{
    service::renderer::ref().clear();
    service::entity::ref().view<SpriteStatus, Vec2f>().each
    (
        [](const Entity entity, SpriteStatus& component, Vec2f& position)
        {
            if (component.isVisible() && component.isReady())
            {
                component.draw(position);
            }
        }
    );

}

void GraphicsSystem::swap()
{

    service::renderer::ref().swap();

}

void GraphicsSystem::destroyComponent(EntityRegistry& registry, Entity entity)
{
    service::resource::ref().release(registry.get<SpriteStatus>(entity).sprite);
}

Camera* GraphicsSystem::getCamera()
{
	return &camera;
}


void GraphicsSystem::end()
{
    service::renderer::ref().end();
}
