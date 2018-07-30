#include "world_entity.hpp"
#include "entity.inl"

namespace 
{
    std::string build_root_name_from_id(const std::string& id)
    {
        auto b = id.rfind('.');
        auto a = id.rfind('/', b) + 1;

        return std::string(&id[a], b - a);
    }
}

bool core::resource::LoaderProxy<WorldEntity::Resource>::load_synchronously(WorldEntity::Resource* ptr)
{
    ptr->m_status = WorldEntity::Status::STORED;
    ptr->m_final.entity = ~Entity{};
    
    return false;
}
bool core::resource::LoaderProxy<WorldEntity::Resource>::unload_synchronously(WorldEntity::Resource* ptr)
{   
    auto root = build_root_name_from_id(ptr->m_id);

    core::serialization::EntityMediator{ ptr->m_final.entity }.save(ptr->m_intermediate.output(root.c_str()));

    ptr->m_intermediate.finalize_output();

    service::entity::ref().destroy(ptr->m_final.entity);

    return false;
}
bool core::resource::LoaderProxy<WorldEntity::Resource>::load_asynchronously(WorldEntity::Resource* ptr)
{
    ptr->m_status = WorldEntity::Status::LOADING;

    ptr->m_intermediate.load(ptr->m_id.c_str());

    return true;
}
bool core::resource::LoaderProxy<WorldEntity::Resource>::unload_asynchronously(WorldEntity::Resource* ptr)
{
    ptr->m_status = WorldEntity::Status::SAVING;

    ptr->m_intermediate.save(ptr->m_id.c_str());
    ptr->m_intermediate.clear();

    ptr->m_status = WorldEntity::Status::STORED;

    return true;
}
bool core::resource::LoaderProxy<WorldEntity::Resource>::synchronize_loaded(WorldEntity::Resource* ptr)
{
    auto root = build_root_name_from_id(ptr->m_id);

    core::serialization::EntityMediator{ ptr->m_final.entity }.load(ptr->m_intermediate.input(root.c_str()));

    ptr->m_intermediate.finalize_input();

    ptr->m_status = WorldEntity::Status::LOADED;

    return true;
}
