
#include "main_character.hpp"
#include "main_character.inl"

#include "position.hpp"
#include "physics/physics_component.hpp"
#include "renderer/sprite_status.hpp"

#include "position.inl"
#include "physics/physics_component.inl"
#include "renderer/sprite_status.inl"

namespace core::serialization
{

    template<typename Input, typename Output>
    std::unordered_map<entt::HashedString::hash_type, typename EntityPropertyRegistry<Input, Output>::Handler> EntityPropertyRegistry<Input, Output>::handlers;

    template<typename Input, typename Output>
    template<typename Type>
    void EntityPropertyRegistry<Input, Output>::registrate(const char* name)
    {
        handlers.emplace
        (
            entt::HashedString{ name },
            Handler
            {
                name,
                EntityRegistry::type<Type>(),
                [](Input& archive, Entity& entity, const char* name)
                {
                    auto& registry = service::entity::ref();
                    
                    typename Type::value_type value{};

                    auto instance(cereal::make_nvp(name, std::move(value)));

                    archive(instance);

                    registry.assign<Type>(entity, std::move(instance.value));
                },
                [](Output& archive, const Entity& entity, const char* name)
                {
                    auto& registry = service::entity::ref();

                    if (registry.has<Type>(entity))
                    {
                        typename Type::value_type& value = registry.get<Type>(entity);

                        archive(cereal::make_nvp(name, value));
                    }
                }
            }
        );
    }

    template<typename Input, typename Output>
    void EntityPropertyRegistry<Input, Output>::load(Input& archive, Entity& entity)
    {
        if (auto* current = archive.getNodeName())
        {
            auto it = handlers.find(entt::HashedString{ current });

            if (it != handlers.end())
            {
                it->second.m_load(archive, entity, current);
            }
        }
    }
    template<typename Input, typename Output>
    void EntityPropertyRegistry<Input, Output>::save(Output& archive, const Entity& entity)
    {
        for (auto& handler : handlers)
        {
            handler.second.m_save(archive, entity, handler.second.m_name);
        }
    }

    template<typename Input, typename Output>
    template<typename Fn>
    void EntityPropertyRegistry<Input, Output>::each(Fn&& fn)
    {
        for (auto& handler : handlers)
        {
            fn(handler.second.m_name, handler.second.m_type);
        }
    }


    template<typename... Tag>
    template<typename Archive>
    void TagMediator<Tag...>::load(Archive& archive)
    {
        auto& registry = service::entity::ref();

        if (!registry.valid(entity))
        {
            entity = registry.create();
        }
        
        registry
            .importer<const char*, cereal::NameValuePair>()
            .tag<Tag...>
            (
                archive, xmlstr::has<Archive>, entity, names
            )
        ;
    }

    template<typename... Tag>
    template<typename Archive>
    void TagMediator<Tag...>::save(Archive& archive) const
    {
        auto& registry = service::entity::ref();

        registry
            .exporter<const char*, cereal::NameValuePair>()
            .tag<Tag...>
            (
                archive, xmlstr::has<Archive>, entity, names
            )
        ;
    }

    template<typename Archive>
    void EntityMediator::load(Archive& archive)
    {
        auto& registry = service::entity::ref();

        entity = registry.create();

        registry
            .importer<const char*, cereal::NameValuePair>()
            .tag<MainCharacter>
            (
                archive, xmlstr::has<Archive>, entity, xmlstr::tags
            )
            .component<Position,SpriteStatus,PhysicsComponent>
            (
                archive, xmlstr::has<Archive>, entity, xmlstr::components
            )
        ;
    }

    template<typename Archive>
    void EntityMediator::save(Archive& archive) const
    {
        auto& registry = service::entity::ref();

        registry
            .exporter<const char*, cereal::NameValuePair>()
            .tag<MainCharacter>
            (
                archive, entity, xmlstr::tags
            )
            .component<Position,SpriteStatus,PhysicsComponent>
            (
                archive, entity, xmlstr::components
            )
        ;
    }

    template<typename Archive>
    void EntitiesMediator::load(Archive& archive)
    {
        auto& registry = service::entity::ref();
        auto importer = registry.importer<const char*, cereal::NameValuePair>();

        size_t size;
        archive(cereal::make_size_tag(size));

        entities.resize(size);

        for (auto& entity : entities)
        {
            entity = registry.create();

            archive.setNextName("entity");
            archive.startNode();
            importer
                .tag<MainCharacter>
                (
                    archive, xmlstr::has<Archive>, entity, xmlstr::tags
                )
                .component<Position, SpriteStatus, PhysicsComponent>
                (
                    archive, xmlstr::has<Archive>, entity, xmlstr::components
                )
            ;
            archive.finishNode();
        }
    }

    template<typename Archive>
    void EntitiesMediator::save(Archive& archive) const
    {
        auto& registry = service::entity::ref();
        auto exporter = registry.exporter<const char*, cereal::NameValuePair>();

        //archive(cereal::make_size_tag(entities.size()));

        for (auto& entity : entities)
        {
            archive.setNextName("entity");
            archive.startNode();
            exporter
                .tag<MainCharacter>
                (
                    archive, entity, xmlstr::tags
                )
                .component<Position, SpriteStatus, PhysicsComponent>
                (
                    archive, entity, xmlstr::components
                )
            ;
            archive.finishNode();
        }
    }
}
