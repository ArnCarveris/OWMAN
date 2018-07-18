#include "world_cell.hpp"
#include "entity_factory.hpp"
#include "i_world_streamer.hpp"
#include "engine.hpp"


bool core::resource::LoaderProxy<WorldCell::Resource>::load_synchronously(WorldCell::Resource* ptr)
{
    ptr->m_status = WorldCell::Status::STORED;
    ptr->m_final.entities.clear();

    return false;
}
bool core::resource::LoaderProxy<WorldCell::Resource>::unload_synchronously(WorldCell::Resource* ptr)
{
    auto* doc = ptr->m_intermediate.getDocument();

    doc->remove_all_attributes();
    doc->remove_all_nodes();
    doc->clear();

    const char* str_cell = doc->allocate_string("cell");
    
    if (ptr->m_final.entities.empty())
    {
        const char* str_space = doc->allocate_string(" ");
        auto* root = doc->allocate_node(rapidxml::node_element, str_cell, str_space);
        doc->append_node(root);
    }
    else
    {
        auto* root = doc->allocate_node(rapidxml::node_element, str_cell);
        doc->append_node(root);

        for (auto& it : ptr->m_final.entities)
        {
            root->append_node(service::entity::ref().createXmlNode(it, doc));

            service::entity::ref().destroyEntity(it);
        }

        ptr->m_final.entities.clear();
    }

    return false;
}
bool core::resource::LoaderProxy<WorldCell::Resource>::load_asynchronously(WorldCell::Resource* ptr)
{
    ptr->m_status = WorldCell::Status::LOADING;

    ptr->m_intermediate.load(ptr->m_id.c_str(), "cell");

    return true;
}
bool core::resource::LoaderProxy<WorldCell::Resource>::unload_asynchronously(WorldCell::Resource* ptr)
{
    ptr->m_status = WorldCell::Status::SAVING;

    ptr->m_intermediate.save(ptr->m_id.c_str());
    ptr->m_intermediate.free();

    ptr->m_status = WorldCell::Status::STORED;

    return true;
}
bool core::resource::LoaderProxy<WorldCell::Resource>::synchronize_loaded(WorldCell::Resource* ptr)
{
    auto *node = ptr->m_intermediate.getNode();

    service::engine::ref().getPositionSystem()->setRelativeCell(ptr->m_final.position);

    node = node->first_node("entity");

    while (node != 0)
    {
        auto ent = service::entity::ref().createEntity(node);

        ptr->m_final.entities.push_back(ent);

        node = node->next_sibling();
    }
    
    ptr->m_status = WorldCell::Status::LOADED;

    return true;
}


