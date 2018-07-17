#include "renderer/graphics_system.hpp"
#include "physics/physics_system.hpp"
#include "position_system.hpp"
#include "entity_factory.hpp"
#include "world_window.hpp"
#include "world_streamer.hpp"
#include "test_world_streamer.hpp"
#include "IO/event_handler.hpp"

#ifndef ENGINE
#define ENGINE

/** \brief encapsulates all systems
 * Has the main loop. Contains all systems like graphics system
 * and physics system
 */
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

#endif
