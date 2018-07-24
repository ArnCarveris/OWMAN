
#include "position.hpp"
#include "physics/physics_component.hpp"
#include "renderer/sprite_status.hpp"

#include "position.inl"
#include "physics/physics_component.inl"
#include "renderer/sprite_status.inl"


namespace core::serialization
{
    template<typename Archive>
    void EntityMediator::load(Archive& archive)
    {
        auto& registry = service::entity::ref().registry;

        entity = registry.create();

        registry
            .importer<const char*, cereal::NameValuePair>()
            .component<Position,SpriteStatus,PhysicsComponent>
            (
                archive, 
                entity, 
                {
                    xmlstr::position,
                    xmlstr::graphics,
                    xmlstr::physics
                }
            )
        ;
    }

    template<typename Archive>
    void EntityMediator::save(Archive& archive) const
    {
        auto& registry = service::entity::ref().registry;

        registry
            .exporter<const char*, cereal::NameValuePair>()
            .component<Position,SpriteStatus,PhysicsComponent>
            (
                archive, 
                entity, 
                {
                    xmlstr::position,
                    xmlstr::graphics,
                    xmlstr::physics
                }
            )
        ;
    }

    template<typename Archive>
    void EntitiesMediator::load(Archive& archive)
    {
        auto& registry = service::entity::ref().registry;
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
                archive, entity,
                {
                    xmlstr::position,
                    xmlstr::graphics,
                    xmlstr::physics
                }
            );
            archive.finishNode();
        }
    }

    template<typename Archive>
    void EntitiesMediator::save(Archive& archive) const
    {
        auto& registry = service::entity::ref().registry;
        auto exporter = registry.exporter<const char*, cereal::NameValuePair>();

        //archive(cereal::make_size_tag(entities.size()));

        for (auto& entity : entities)
        {
            archive.setNextName("entity");
            archive.startNode();
            exporter.component<Position, SpriteStatus, PhysicsComponent>
            (
                archive, entity,
                {
                    xmlstr::position,
                    xmlstr::graphics,
                    xmlstr::physics
                }
            );
            archive.finishNode();
        }
    }
}
