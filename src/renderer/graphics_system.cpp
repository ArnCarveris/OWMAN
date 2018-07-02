#include "graphics_system.hpp"
#include "graphics_component.hpp"
#include "sprite_status.hpp"
#include "color.hpp"
#include "../resource_manager/resource_manager.hpp"
#include <iostream>
#include <algorithm>
#include "../util/time_conversions.hpp"
#include "../util/xmlstr.hpp"

using namespace std;

GraphicsSystem::GraphicsSystem
(
	string windowTitle,
	unsigned int width, unsigned int height,
	bool fullScreen
) {
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

SpriteStatus* GraphicsSystem::createComponent(rapidxml::xml_node<>* node)
{
    rapidxml::xml_node<> *sprite_node = node->first_node(xmlstr::sprite);
    string spriteName = sprite_node->value();
    rapidxml::xml_node<> *width_graphics_node = node->first_node(xmlstr::width);
    float width_graphics = atof(width_graphics_node->value());
    rapidxml::xml_node<> *height_graphics_node = node->first_node(xmlstr::height);
    float height_graphics = atof(height_graphics_node->value());
    rapidxml::xml_node<> *priority_node = node->first_node(xmlstr::priority);
    
    SpriteStatus* spriteStatus = new SpriteStatus(this, service::resource::ref().obtain<Sprite::Resource>(core::resource::ID{ spriteName.c_str() }));
    spriteStatus->setScale(Vec2f(width_graphics, height_graphics));    // < TODO


    if (priority_node)
    {
        int priority = atoi(priority_node->value());
        spriteStatus->setPriority(priority);
    }

    components.insert(spriteStatus);

	return spriteStatus;
}


void GraphicsSystem::destroyGraphicsComponent(GraphicsComponent* graphicsComponent)
{
    graphicsComponent->destroyDispatcher();

    components.erase(graphicsComponent);
}

Camera* GraphicsSystem::getCamera()
{
	return &camera;
}


void GraphicsSystem::end()
{
    service::renderer::ref().end();
}
