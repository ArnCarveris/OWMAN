
#include "main_character.hpp"
#include "main_character.inl"

#include "position.hpp"
#include "physics/physics_component.hpp"
#include "renderer/sprite_status.hpp"

#include "position.inl"
#include "physics/physics_component.inl"
#include "renderer/sprite_status.inl"

namespace xmlstr
{
    template<typename Archive>
    bool has(Archive& archive, const char* name)
    {
        const char* current = archive.getNodeName();

        return current != nullptr && !strcmp(current, name);
    }

    const std::array<const char*, 1> tags = {
        "main_character"
    };

    const std::array<const char*, 3> components = {
        position,
        graphics,
        physics
    };
}

namespace core::serialization
{
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
