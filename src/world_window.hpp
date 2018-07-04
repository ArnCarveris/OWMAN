#include <vector>
#include <map>
#include "position.hpp"
#include "world_cell.hpp"
#include "entity.hpp"

#ifndef WORLD_WINDOW
#define WORLD_WINDOW

/**
 * Collection of active cells
 */
class WorldWindow
{

	friend class WorldStreamer;

	std::map<Vec2i, core::resource::Handle<WorldCell::Resource>> cells;

	unsigned int windowSize;

public:

	WorldWindow();

	WorldWindow(unsigned int windowSize);

    std::vector<Entity> getEntities()const;

	~WorldWindow();

};

#endif
