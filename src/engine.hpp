

#ifndef ENGINE
#define ENGINE

#include "events.hpp"
#include "world_window.hpp"
#include "world_entity.hpp"
#include <string>

/** \brief encapsulates all systems
 * Has the main loop. Contains all systems like graphics system
 * and physics system
 */
class PositionSystem;
class GraphicsSystem;
class PhysicsSystem;
class WorldStreamer;

class Engine
{

	WorldWindow worldGrid;

    core::resource::Handle<WorldEntity::Resource> mainCharacterResource;


	bool end = false;
	int fps = 60;

public:

	Engine(std::string initFile, std::string worldFolder);

	void init();

	void mainLoop();

    void prepare(const WorldRepositionEvent& event);
    void finalize(const WorldRepositionEvent& event);

    PositionSystem* getPositionSystem();
    GraphicsSystem* getGraphicsSystem();
    PhysicsSystem* getPhysicsSystem();
    WorldStreamer* getWorldStreamer();

	void endGame();

	~Engine();

};

namespace service
{
    using engine = entt::ServiceLocator<Engine>;
}

#endif
