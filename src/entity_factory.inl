
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

}
