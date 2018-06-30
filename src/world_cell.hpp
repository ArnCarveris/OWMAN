 #include <vector>

#ifndef ENTITY
class Entity;
#endif

#ifndef WORLD_CELL
#define WORLD_CELL

class WorldCell
{
public:
    std::vector<Entity*> entities;
};


#endif
