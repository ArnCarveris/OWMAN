#include <vector>
#include "math/vec2i.hpp"
#include "resource_manager/resource_xml.hpp"
#include "resource_manager/resource_manager.hpp"

#ifndef ENTITY
class Entity;
#endif

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

    using Resource = core::resource::Data<ResourceXml, WorldCell, Status>;

public:
    std::vector<Entity*> entities;

    Vec2i position;
};


#endif
