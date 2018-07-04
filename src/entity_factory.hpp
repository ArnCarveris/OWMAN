#include "entity.hpp"
#include "main_character.hpp"
#include <rapidxml.hpp>

#include <entt/locator/locator.hpp>

#ifndef ENGINE
class Engine;
#endif

#ifndef ENTITY_FACTORY
#define ENTITY_FACTORY

class EntityFactory
{

	Engine* myEngine;

public:
    EntityRegistry registry;

	EntityFactory(Engine* engine);

	/**
	 * \brief Create an entity
	 * \param node The root xml node
	 * \param toCenter Number of cells to the center of the world window
	 * \return A pointer to the entity. Use destroyEntity() to release.
	 **/
	Entity createEntity(rapidxml::xml_node<> *node, const Vec2i& toCenter);


    /**
	 * \brief Create a main character
	 * \param node The root xml node
	 * \return A pointer to the entity. Use destroyEntity() to release.
	 **/
    Entity createMainCharacter(rapidxml::xml_node<> *node);

    rapidxml::xml_node<>* createXmlNode(Entity entity, rapidxml::xml_document<>* doc, float cellSize);

	void destroyEntity(Entity entity);

};

namespace service
{
    using entity = entt::ServiceLocator<EntityFactory>;
}

#endif
