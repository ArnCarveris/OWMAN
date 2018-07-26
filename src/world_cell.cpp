#include "world_cell.hpp"
#include "entity.hpp"
#include "entity.inl"
#include "i_world_streamer.hpp"
#include "engine.hpp"
#include "position_system.hpp"



bool core::resource::LoaderProxy<WorldCell::Resource>::load_synchronously(WorldCell::Resource* ptr)
{
    ptr->m_status = WorldCell::Status::STORED;
    ptr->m_final.entities.clear();

    return false;
}
bool core::resource::LoaderProxy<WorldCell::Resource>::unload_synchronously(WorldCell::Resource* ptr)
{
    core::serialization::EntitiesMediator{ ptr->m_final.entities }.save(ptr->m_intermediate.output("cell"));

    ptr->m_intermediate.finalize_output();

    for (auto& entity : ptr->m_final.entities)
    {
        service::entity::ref().destroy(entity);
    }

    ptr->m_final.entities.clear();

    return false;
}
bool core::resource::LoaderProxy<WorldCell::Resource>::load_asynchronously(WorldCell::Resource* ptr)
{
    ptr->m_status = WorldCell::Status::LOADING;

    ptr->m_intermediate.load(ptr->m_id.c_str());

    return true;
}
bool core::resource::LoaderProxy<WorldCell::Resource>::unload_asynchronously(WorldCell::Resource* ptr)
{
    ptr->m_status = WorldCell::Status::SAVING;

    ptr->m_intermediate.save(ptr->m_id.c_str());
    ptr->m_intermediate.clear();

    ptr->m_status = WorldCell::Status::STORED;

    return true;
}
bool core::resource::LoaderProxy<WorldCell::Resource>::synchronize_loaded(WorldCell::Resource* ptr)
{
    service::engine::ref().getPositionSystem()->setRelativeCell(ptr->m_final.position);

    core::serialization::EntitiesMediator{ ptr->m_final.entities }.load(ptr->m_intermediate.input("cell"));

    ptr->m_intermediate.finalize_input();
    
    ptr->m_status = WorldCell::Status::LOADED;

    return true;
}


