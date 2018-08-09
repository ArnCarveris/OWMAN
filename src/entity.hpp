#pragma once

#include <unordered_map>

#include <entt/core/hashed_string.hpp>
#include <entt/core/property.hpp>
#include <entt/entity/registry.hpp>
#include <entt/locator/locator.hpp>

using EntityRegistry = entt::DefaultRegistry;
using Entity = EntityRegistry::entity_type;

namespace core::serialization
{
    template<typename Input, typename Output>
    class EntityPropertyRegistry : entt::PropertyRegistry<EntityPropertyRegistry<Input, Output>> {
        using load_fn_type = void(*)(Input&, Entity&, const char*);
        using save_fn_type = void(*)(Output&, const Entity&, const char*);

        struct Handler {
            const char* m_name;
            EntityRegistry::component_type m_type;
            load_fn_type m_load;
            save_fn_type m_save;
        };

        static std::unordered_map<entt::HashedString::hash_type, Handler> handlers;
    public:
        template<typename Value, char... C>
        using type = entt::Property<EntityPropertyRegistry, Value, C...>;

        template<typename Type>
        static void registrate(const char* name);

        static void load(Input& archive, Entity& entity);
        static void save(Output& archive, const Entity& entity);

        template<typename Fn>
        static void each(Fn&& fn);
    };

    template<typename... Tag>
    struct TagMediator
    {
        using Names = std::array<const char*, sizeof...(Tag)>;

        TagMediator() = delete;
        TagMediator(const TagMediator&) = delete;

        Entity& entity;
        const Names names;

        template<typename Archive>
        void load(Archive& archive);

        template<typename Archive>
        void save(Archive& archive) const;
    };

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