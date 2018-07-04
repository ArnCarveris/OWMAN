#include "graphics_system.hpp"
#include "sprite_status.hpp"
#include "color.hpp"
#include "../resource_manager/resource_manager.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../entity_factory.hpp"
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
    service::entity::ref().registry.destruction<SpriteStatus>().connect<GraphicsSystem, &GraphicsSystem::destroyComponent>(this);
}

void GraphicsSystem::setFullScreen(bool b)
{
    service::renderer::ref().setFullScreen(b);
}

void GraphicsSystem::update(unsigned int delta)
{

    float deltaSeconds = ticksToSeconds(delta);

    for (auto& it : service::entity::ref().registry.view<SpriteStatus>(entt::raw_t{}))
	{
		it.update(deltaSeconds);
	}

    service::entity::ref().registry.sort<SpriteStatus>
    (
        [](const SpriteStatus& gc1, const SpriteStatus& gc2) -> bool
        {
            return gc1.getPriority() < gc2.getPriority();
        }
    );
    service::entity::ref().registry.sort<Vec2f, SpriteStatus>();
}

void GraphicsSystem::draw()
{
    service::renderer::ref().clear();
    service::entity::ref().registry.view<SpriteStatus, Vec2f>().each
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




void GraphicsSystem::assignComponent(EntityRegistry& registry, Entity entity, rapidxml::xml_node<>* node)
{
    rapidxml::xml_node<> *sprite_node = node->first_node(xmlstr::sprite);
    string spriteName = sprite_node->value();
    rapidxml::xml_node<> *width_graphics_node = node->first_node(xmlstr::width);
    float width_graphics = atof(width_graphics_node->value());
    rapidxml::xml_node<> *height_graphics_node = node->first_node(xmlstr::height);
    float height_graphics = atof(height_graphics_node->value());
    rapidxml::xml_node<> *priority_node = node->first_node(xmlstr::priority);
    
    SpriteStatus& spriteStatus = registry.assign<SpriteStatus>(entity, this, service::resource::ref().obtain<Sprite::Resource>(core::resource::ID{ spriteName.c_str() }));
    spriteStatus.setScale(Vec2f(width_graphics, height_graphics));    // < TODO


    if (priority_node)
    {
        int priority = atoi(priority_node->value());
        spriteStatus.setPriority(priority);
    }
}

void GraphicsSystem::destroyComponent(EntityRegistry& registry, Entity entity)
{
    service::resource::ref().release(registry.get<SpriteStatus>(entity).sprite);
}

rapidxml::xml_node<>* GraphicsSystem::createXmlNode(EntityRegistry& registry, Entity entity, rapidxml::xml_document<>* doc)
{
    if (!registry.has<SpriteStatus>(entity))
    {
        return nullptr;
    }

    auto& component = registry.get<SpriteStatus>(entity);

    rapidxml::xml_node<>* root = doc->allocate_node(rapidxml::node_element, xmlstr::graphics);
    stringstream ss;
    // scaleX
    ss << component.scale.x;
    const char* scaleX = doc->allocate_string(ss.str().c_str());
    // clear
    ss.clear();
    ss.str(string());
    //scaleY
    ss << component.scale.y;
    const char* scaleY = doc->allocate_string(ss.str().c_str());
    // clear
    ss.clear();
    ss.str(string());
    // priority
    ss << component.priority;
    const char* prio = doc->allocate_string(ss.str().c_str());
    // allocate strings

    // allocate nodes
    rapidxml::xml_node<>* node_width = doc->allocate_node(rapidxml::node_element, xmlstr::width, scaleX);
    rapidxml::xml_node<>* node_height = doc->allocate_node(rapidxml::node_element, xmlstr::height, scaleY);
    rapidxml::xml_node<>* node_priority = doc->allocate_node(rapidxml::node_element, xmlstr::priority, prio);
    // attach nodes
    root->append_node(node_width);
    root->append_node(node_height);
    root->append_node(node_priority);

    const char* spriteName = *component.sprite;
    const char* str_spriteName = doc->allocate_string(spriteName);
    rapidxml::xml_node<>* node_sprite = doc->allocate_node(rapidxml::node_element, xmlstr::sprite, str_spriteName);
    root->prepend_node(node_sprite);

    return root;
}

Camera* GraphicsSystem::getCamera()
{
	return &camera;
}


void GraphicsSystem::end()
{
    service::renderer::ref().end();
}
