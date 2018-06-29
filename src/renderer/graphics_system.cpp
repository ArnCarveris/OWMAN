#include "graphics_system.hpp"
#include "graphics_component.hpp"
#include "sprite_status.hpp"
#include "color.hpp"
#include "../resource_manager/resource_manager.hpp"
#include <iostream>
#include <algorithm>
#include "../util/time_conversions.hpp"

using namespace std;

GraphicsSystem::GraphicsSystem
(
	string windowTitle,
	unsigned int width, unsigned int height,
	bool fullScreen
)
:
spriteManager(this)
{
    service::renderer::set
    (
        windowTitle,
        width, height,
        fullScreen
    );
}

void GraphicsSystem::setFullScreen(bool b)
{
    service::renderer::ref().setFullScreen(b);
}

void GraphicsSystem::update(unsigned int delta)
{

    float deltaSeconds = ticksToSeconds(delta);

	spriteManager.update(deltaSeconds);

	set<GraphicsComponent*>::iterator it;
	for( it=components.begin(); it != components.end(); ++it )
	{

		(*it)->update(deltaSeconds);

	}

}

void GraphicsSystem::draw()
{

    vector<GraphicsComponent*> vec(components.begin(), components.end());

    // sort by priority
    // TODO: avoid sorting in each frame, but its difficult to implement
    sort
    (
        vec.begin(),
        vec.end(),
        [](GraphicsComponent* gc1, GraphicsComponent* gc2) -> bool
        {
            return gc1->getPriority() < gc2->getPriority();
        }
    );

    service::renderer::ref().clear();
	vector<GraphicsComponent*>::iterator it;
	for( it=vec.begin(); it != vec.end(); ++it )
	{

        if( (*it)->isVisible() )
        {
            (*it)->draw();
        }

	}

}

void GraphicsSystem::swap()
{

    service::renderer::ref().swap();

}

SpriteStatus* GraphicsSystem::instanceSprite(std::string name, const Vec2f& scale)
{
	SpriteStatus* sprite = spriteManager.instanceSprite(name);
	sprite->setScale(scale);    // < TODO
	components.insert(sprite);
	return sprite;
}

void GraphicsSystem::destroySpriteInstance(SpriteStatus* sprite)
{

    components.erase(sprite);
    spriteManager.releaseSpriteInstance(sprite);

}

void GraphicsSystem::destroyGraphicsComponent(GraphicsComponent* graphicsComponent)
{

    components.erase(graphicsComponent);
    graphicsComponent->destroyDispatcher();

}

Camera* GraphicsSystem::getCamera()
{
	return &camera;
}

SpriteManager* GraphicsSystem::getSpriteManager()
{
    return &spriteManager ;
}

void GraphicsSystem::end()
{
    service::renderer::ref().end();
}
