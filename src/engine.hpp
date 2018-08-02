

#ifndef ENGINE
#define ENGINE

#include "events.hpp"
#include "world_window.hpp"
#include "world_entity.hpp"
#include <string>

class Engine
{

	WorldWindow worldGrid;

    std::vector<core::resource::Handle<WorldEntity::Resource>> world_entities;


	bool end = false;
	int fps = 60;

public:

	Engine(std::string initFile, std::string worldFolder);

	void init();

	void mainLoop();

    void prepare(const WorldRepositionEvent& event);
    void finalize(const WorldRepositionEvent& event);

	void endGame();

	~Engine();

};

namespace service
{
    using engine = entt::ServiceLocator<Engine>;
}

#endif
