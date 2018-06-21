#include "engine.hpp"
#include "resource_manager/resource_cell.hpp"
#include "resource_manager/resource_text.hpp"
#include "resource_manager/resource_texture.hpp"
#include "resource_manager/resource_manager.hpp"
#include <rapidxml.hpp>
#include "util/file_to_string.hpp"
#include "physics/physics_component.hpp"
#include "renderer/graphics_component.hpp"
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

    service::input::set(this);
    service::resource::set();
    service::world_streamer::set<WorldStreamer>(worldFolder, &entityFactory, cellSize, windowSize);

	entityFactory = EntityFactory(this);

    // init systems
	graphicsSystem = new GraphicsSystem( title, xResolution, yResolution, fullscreen );
	Camera* camera = graphicsSystem->getCamera();
	camera->setWidth(xResolution/2);
	camera->setHeight(yResolution/2);

	physicsSystem = new PhysicsSystem();

	delete initFileText;

    // main character

    stringstream ss;
    ss << worldFolder << "/" << "main_character.xml";
    string mcFileName = ss.str();

    char* mcFileText = fileToString(mcFileName.c_str());

    assert(mcFileText && "main_character.xml not found");

    xml_document<> mcDoc;
    mcDoc.parse<0>( mcFileText );

    node = mcDoc.first_node("main_character");

    mainCharacter = entityFactory.createMainCharacter(node);


    delete mcFileText;

}

void Engine::init()
{

    SDL_Init(SDL_INIT_TIMER);
    service::resource::ref().deliver<ResourceCell>();
    service::resource::ref().deliver<ResourceText>();
    service::resource::ref().deliver<ResourceTexture>();
    service::resource::ref().launch();

    service::world_streamer::ref().init
    (
        mainCharacter->getCell(),
        mainCharacter->getPosition()
    );

}

void Engine::mainLoop()
{

    unsigned prevTicks = SDL_GetTicks();

    while( !end )
    {

        unsigned ticks = SDL_GetTicks();

        service::input::ref().poll();

        // update physics
        physicsSystem->update( ticks - prevTicks );

        // update world streamer

        service::world_streamer::ref().update( mainCharacter->getPosition(), mainCharacter );

        // get list of entities
        vector<Entity*> entities = service::world_streamer::ref().getEntities();

        // move graphics components to match physics components
        // (update position of graphics components)
        for(unsigned int i=0; i<entities.size(); i++)
        {
            Entity* entity = entities[i];
            if
            (
                entity->getGraphicsComponent() != 0 &&
                entity->getPhysicsComponent() != 0
            )
            {
                GraphicsComponent* gc = entity->getGraphicsComponent();
                PhysicsComponent* pc = entity->getPhysicsComponent();
                gc->setPosition( pc->getPosition() );
            }
        }

        GraphicsComponent* gc = mainCharacter->getGraphicsComponent();
        PhysicsComponent* pc = mainCharacter->getPhysicsComponent();
        gc->setPosition( pc->getPosition() );


        // follow main character with the camera
        graphicsSystem->getCamera()->setPosition( -mainCharacter->getPosition() );

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

GraphicsSystem* Engine::getGraphicsSystem()
{
	return graphicsSystem;
}

PhysicsSystem* Engine::getPhysicsSystem()
{
    return physicsSystem;
}

MainCharacter* Engine::getMainCharacter()
{
    return mainCharacter;
}

float Engine::getCellSize()const
{
    return service::world_streamer::ref().getCellSize();
}

void Engine::endGame()
{
    service::world_streamer::ref().end();
    graphicsSystem->end();
    end = true;
}

Engine::~Engine()
{
    if( graphicsSystem ) delete graphicsSystem;
    if( mainCharacter) delete mainCharacter;

    service::resource::reset();
    service::world_streamer::reset();
}
