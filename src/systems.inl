#include "entity.hpp"
#include "entity.inl"

#include "position_system.hpp"
#include "position_system.inl"

#include "renderer/graphics_system.hpp"
#include "renderer/graphics_system.inl"

#include "physics/physics_system.hpp"
#include "physics/physics_system.inl"

#include "world_streamer.hpp"
#include "world_streamer.inl"


template<typename Archive>
void Systems::load(Archive& archive)
{
    auto entity = service::entity::ref().attachee<Systems>();

    core::serialization::TagMediator
    <
        PositionSystem,
        GraphicsSystem,
        PhysicsSystem,
        WorldStreamer
     > {
        entity,
        {
            "positioning",
            "graphics",
            "physics",
            "world_streamer"
        }
    }.load(archive);
}

template<typename Archive>
void Systems::save(Archive& ar) const { }

