

#ifndef ENGINE
#define ENGINE

#include "events.hpp"
#include "world_window.hpp"
#include <string>

/** \brief encapsulates all systems
 * Has the main loop. Contains all systems like graphics system
 * and physics system
 */
class PositionSystem;
class GraphicsSystem;
class PhysicsSystem;

class Engine
{

	WorldWindow worldGrid;

	// Systems
    PositionSystem* positionSystem;
	GraphicsSystem* graphicsSystem;
	PhysicsSystem* physicsSystem;

	bool end;
	int fps;

public:

	Engine(std::string initFile, std::string worldFolder);

	void init();

	void mainLoop();

    void prepare(const WorldRepositionEvent& event);
    void finalize(const WorldRepositionEvent& event);

    PositionSystem* getPositionSystem();
    GraphicsSystem* getGraphicsSystem();
    PhysicsSystem* getPhysicsSystem();

	void endGame();

	~Engine();

};

namespace service
{
    using engine = entt::ServiceLocator<Engine>;
}

#endif
