#include "entity.hpp"
#include <rapidxml.hpp>

#include <entt/locator/locator.hpp>

#include "math/vec2i.hpp"

#ifndef ENGINE
class Engine;
#endif

#ifndef ENTITY_FACTORY
#define ENTITY_FACTORY

namespace core::serialization
{
    struct EntityMediator
    {
        EntityMediator() = delete;
        EntityMediator(const EntityMediator&) = delete;

        Entity& entity;

        template<typename Archive>
        void load(Archive& archive);

        template<typename Archive>
        void save(Archive& archive) const;
    };
}

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
	Entity createEntity(rapidxml::xml_node<> *node);

    rapidxml::xml_node<>* createXmlNode(Entity entity, rapidxml::xml_document<>* doc);

};

namespace service
{
    using entity = entt::ServiceLocator<EntityFactory>;
}

#endif
