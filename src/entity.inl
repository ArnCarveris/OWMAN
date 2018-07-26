
#include "position.hpp"
#include "physics/physics_component.hpp"
#include "renderer/sprite_status.hpp"

#include "position.inl"
#include "physics/physics_component.inl"
#include "renderer/sprite_status.inl"

namespace xmlstr::component
{
    template<typename Archive>
    bool has(Archive& archive, const char* name)
    {
        const char* current = archive.getNodeName();

        return current != nullptr && !strcmp(current, name);
    }

    const std::array<const char*, 3> list = {
        position,
        graphics,
        physics
    };
}

namespace core::serialization
{
    template<typename Archive>
    void EntityMediator::load(Archive& archive)
    {
        auto& registry = service::entity::ref();

        entity = registry.create();

        registry
            .importer<const char*, cereal::NameValuePair>()
            .component<Position,SpriteStatus,PhysicsComponent>
            (
                archive, xmlstr::component::has<Archive>, entity, xmlstr::component::list
            )
        ;
    }

    template<typename Archive>
    void EntityMediator::save(Archive& archive) const
    {
        auto& registry = service::entity::ref();

        registry
            .exporter<const char*, cereal::NameValuePair>()
            .component<Position,SpriteStatus,PhysicsComponent>
            (
                archive, entity, xmlstr::component::list
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
            importer.component<Position, SpriteStatus, PhysicsComponent>
            (
                archive, xmlstr::component::has<Archive>, entity, xmlstr::component::list
            );
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
            exporter.component<Position, SpriteStatus, PhysicsComponent>
            (
                archive, entity, xmlstr::component::list
            );
            archive.finishNode();
        }
    }
}
