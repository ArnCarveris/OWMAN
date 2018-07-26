#include "engine.hpp"
#include "events.hpp"
#include "resource_manager/resource_manager.hpp"
#include "renderer/texture.hpp"
#include "renderer/sprite.hpp"
#include "main_character.hpp"
#include "IO/event_handler.hpp"
#include "entity_factory.hpp"
#include "entity_factory.inl"
#include "position_system.hpp"
#include "renderer/graphics_system.hpp"
#include "physics/physics_system.hpp"
#include "world_streamer.hpp"

#include <rapidxml.hpp>
#include "util/file_to_string.hpp"
#include "physics/physics_component.hpp"
#include "renderer/sprite_status.hpp"
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>


using namespace std;
using namespace rapidxml;

Engine::Engine(std::string initFile, std::string worldFolder)
{

    end = false;
    fps = 60;

	char* initFileText = fileToString( initFile.c_str() );

	// check errors
	if( initFileText == 0 )
	{

		cerr << "Error loading initialization file: "
		<< initFile << endl;

		return;

	}

	xml_document<> doc;
	doc.parse<0>( initFileText );

	xml_node<>* node;

	node = doc.first_node("window_title");
	string title = node->value();

	node = doc.first_node("fullscreen");
	string fullscreenString = node->value();
	bool fullscreen =
	(
		(fullscreenString == string("true")) ?
		true :
		false
	);


	node = doc.first_node("x_resolution");
	int xResolution = atof( node->value() );


	node = doc.first_node("y_resolution");
	int yResolution = atof( node->value() );

	node = doc.first_node("cell_size");
	float cellSize = atof( node->value() );

	node = doc.first_node("window_size");
	int windowSize = atoi( node->value() );

    delete initFileText;

    service::input::set(this);
    service::entity::set(this);
    service::resource::set();
    service::dispatcher::set();
    service::world_streamer::set<WorldStreamer>(worldFolder, cellSize, windowSize);


    service::dispatcher::ref().sink<WorldRepositionEvent>().connect<Engine, &Engine::finalize>(this);

    // init systems
    positionSystem = new PositionSystem();

	graphicsSystem = new GraphicsSystem( title, xResolution, yResolution, fullscreen );
	Camera* camera = graphicsSystem->getCamera();
	camera->setWidth(xResolution/2);
	camera->setHeight(yResolution/2);

	physicsSystem = new PhysicsSystem();

    service::dispatcher::ref().sink<WorldRepositionEvent>().connect<Engine, &Engine::prepare>(this);
}



void Engine::init()
{

    SDL_Init(SDL_INIT_TIMER);

    service::resource::ref()
        .deliver<Texture::Resource>()
        .deliver<WorldCell::Resource>()
        .deliver<Sprite::Resource>()
        .launch()
    ;

    // main character
    Entity mainCharacter;
    {
        std::string path = service::world_streamer::ref().getWorldFolder() + "/main_character.xml";

        getPositionSystem()->setRelativeCell(Vec2i(0,0));

        ResourceArchive<cereal::XMLRootInputArchive, cereal::XMLOutputArchive> archive;

        archive.load(path.c_str());

        core::serialization::EntityMediator{ mainCharacter }.load(archive.input("main_character"));
    }
    service::entity::ref().registry.assign<MainCharacter>(entt::tag_t{}, mainCharacter);

    service::world_streamer::ref().init(service::entity::ref().registry.get<Position>(mainCharacter));
}

void Engine::mainLoop()
{

    unsigned prevTicks = SDL_GetTicks();

    while( !end )
    {

        unsigned ticks = SDL_GetTicks();

        service::input::ref().poll();

        service::resource::ref().synchronize();

        // update physics
        physicsSystem->update( ticks - prevTicks );

        // update world streamer

        auto mainCharacter = service::entity::ref().registry.attachee<MainCharacter>();

        service::world_streamer::ref().update( service::entity::ref().registry.get<Vec2f>(mainCharacter));



        // follow main character with the camera
        graphicsSystem->getCamera()->setPosition( -service::entity::ref().registry.get<Vec2f>(mainCharacter));

        // update graphics
        graphicsSystem->update(ticks-prevTicks);

        // draw
        graphicsSystem->draw();
        graphicsSystem->swap();

        unsigned int endTicks = SDL_GetTicks();
		int sleepTicks = 1000/fps - (endTicks-ticks);
		if(sleepTicks > 0)
			SDL_Delay( sleepTicks );

        prevTicks = ticks;

    }

}

void Engine::prepare(const WorldRepositionEvent& event)
{
    service::entity::ref().registry.view<Vec2f>().each
    (
        [delta = event.to - event.from](const Entity entity, Vec2f& position) 
        {
            position += delta;
        }
    );

    auto mainCharacter = service::entity::ref().registry.attachee<MainCharacter>();

    service::entity::ref().registry.get<Vec2f>(mainCharacter) = event.to;
}

void Engine::finalize(const WorldRepositionEvent& event)
{

}

PositionSystem* Engine::getPositionSystem()
{
    return positionSystem;
}
GraphicsSystem* Engine::getGraphicsSystem()
{
	return graphicsSystem;
}

PhysicsSystem* Engine::getPhysicsSystem()
{
    return physicsSystem;
}

void Engine::endGame()
{
    service::world_streamer::ref().end();
    graphicsSystem->end();
    end = true;
}

Engine::~Engine()
{
    if (positionSystem) delete positionSystem;
    if( graphicsSystem ) delete graphicsSystem;

    service::entity::reset();
    service::resource::reset();
    service::dispatcher::reset();
    service::world_streamer::reset();
}
