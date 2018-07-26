#pragma once

#include <entt/entity/registry.hpp>
#include <entt/locator/locator.hpp>

using EntityRegistry = entt::DefaultRegistry;
using Entity = EntityRegistry::entity_type;

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

namespace service
{
    using entity = entt::ServiceLocator<EntityRegistry>;
}