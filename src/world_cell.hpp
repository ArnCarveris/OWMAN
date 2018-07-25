#include <vector>
#include "math/vec2i.hpp"
#include "resource_manager/resource_archive.hpp"
#include "resource_manager/resource_manager.hpp"
#include "entity.hpp"

#ifndef WORLD_CELL
#define WORLD_CELL

class WorldCell
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

    using Resource = core::resource::Data<ResourceArchive<cereal::XMLRootInputArchive, cereal::XMLRootOutputArchive>, WorldCell, Status>;

public:
    std::vector<Entity> entities;

    Vec2i position;
};


#endif
