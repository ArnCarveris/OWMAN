#pragma once

#include "entity.hpp"

#include "resource_manager/resource_archive.hpp"
#include "resource_manager/resource_manager.hpp"


class WorldEntity
{
public:
    enum class Status
    {
        STORED,
        LOADING,
        LOADED,
        SAVING,
        SAVED
    };

    using Resource = core::resource::Data<ResourceXMLRootArchive, WorldEntity, Status>;

public:
    Entity entity;
};
