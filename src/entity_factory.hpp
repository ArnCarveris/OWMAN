#include "entity.hpp"

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

    struct EntitiesMediator
    {
        EntitiesMediator() = delete;
        EntitiesMediator(const EntitiesMediator&) = delete;

        std::vector<Entity>& entities;

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
};

namespace service
{
    using entity = entt::ServiceLocator<EntityFactory>;
}

#endif
