#include "engine.hpp"
#include "events.hpp"
#include "resource_manager/resource_manager.hpp"
#include "renderer/texture.hpp"
#include "renderer/sprite.hpp"
#include "main_character.hpp"
#include "IO/event_handler.hpp"
#include "entity.hpp"
#include "entity.inl"
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
    service::input::set(this);
    service::entity::set();
    service::resource::set();
    service::dispatcher::set();
    service::resource::ref()
        .deliver<Texture::Resource>()
        .deliver<Sprite::Resource>()
        .deliver<WorldEntity::Resource>()
        .deliver<WorldCell::Resource>()
        .launch()
    ;
    {
        ResourceXMLRootArchive archive;

        archive.load(initFile.c_str());

        auto& input = archive.input("init");
       
        {
            string title;
            bool fullscreen;
            int xResolution;
            int yResolution;

            input(cereal::make_nvp("window_title", title));
            input(cereal::make_nvp("fullscreen", fullscreen));
            input(cereal::make_nvp("x_resolution", xResolution));
            input(cereal::make_nvp("y_resolution", yResolution));

            service::dispatcher::ref().sink<WorldRepositionEvent>().connect<Engine, &Engine::finalize>(this);
            {
                // init systems

                auto e = service::entity::ref().create();

                service::entity::ref().assign<PositionSystem>(entt::tag_t{}, e);

                service::entity::ref().assign<GraphicsSystem>(entt::tag_t{}, e, title, xResolution, yResolution, fullscreen);

                service::entity::ref().assign<PhysicsSystem>(entt::tag_t{}, e);
                {
                    float cellSize;
                    int windowSize;

                    input(cereal::make_nvp("cell_size", cellSize));
                    input(cereal::make_nvp("window_size", windowSize));
                    
                    service::entity::ref().assign<WorldStreamer>(entt::tag_t{}, e, worldFolder, cellSize, windowSize);

                }
            }
            service::dispatcher::ref().sink<WorldRepositionEvent>().connect<Engine, &Engine::prepare>(this);
        } {
            input(cereal::make_nvp("main_character", mainCharacterResource));
        }
        archive.finalize_input();
    }
}



void Engine::init()
{
    SDL_Init(SDL_INIT_TIMER);  
}

void Engine::mainLoop()
{
    Entity mainCharacter;

    unsigned ticks;
    unsigned prevTicks = SDL_GetTicks();

    auto frame_start = [&ticks]()
    {
        ticks = SDL_GetTicks();
    };
    auto frame_sleep = [this, &ticks, &prevTicks]()
    {
        unsigned int endTicks = SDL_GetTicks();
        int sleepTicks = 1000 / fps - (endTicks - ticks);
        if (sleepTicks > 0)
            SDL_Delay(sleepTicks);

        prevTicks = ticks;
    };
    auto frame_tick = [&](unsigned dt)
    {
        service::input::ref().poll();

        // update physics
        getPhysicsSystem()->update(dt);
        getPositionSystem()->update();

        // update world streamer

        getWorldStreamer()->update(service::entity::ref().get<Vec2f>(mainCharacter));

        // follow main character with the camera
        getGraphicsSystem()->getCamera()->setPosition(-service::entity::ref().get<Vec2f>(mainCharacter));

        // update graphics
        getGraphicsSystem()->update(dt);

        // draw
        getGraphicsSystem()->draw();
        getGraphicsSystem()->swap();
    };
    {
        while (mainCharacterResource->get_status() != WorldEntity::Status::LOADED)
        {
            frame_start();
            service::resource::ref().synchronize();
            frame_sleep();
        }

        mainCharacter = mainCharacterResource->get().entity;

        service::entity::ref().assign<MainCharacter>(entt::tag_t{}, mainCharacter);

        getWorldStreamer()->init(service::entity::ref().get<Position>(mainCharacter));
    }
    
    while( !end )
    {
        frame_start();

        service::resource::ref().synchronize();

        frame_tick(ticks - prevTicks);

        frame_sleep();
    } 

    {
        service::entity::ref()
            .get<Position>(mainCharacter)
            .setCell(getWorldStreamer()->getWindowPosition());

        service::resource::ref().release(mainCharacterResource);

        getWorldStreamer()->end();
        service::resource::ref().stop();
    }

    while (end)
    {
        frame_start();
        end = service::resource::ref().synchronize();
        frame_sleep();
    }

    getGraphicsSystem()->end();
}

void Engine::prepare(const WorldRepositionEvent& event)
{
    service::entity::ref().view<Vec2f>().each
    (
        [delta = event.to - event.from](const Entity entity, Vec2f& position) 
        {
            position += delta;
        }
    );

    auto mainCharacter = service::entity::ref().attachee<MainCharacter>();

    service::entity::ref().get<Vec2f>(mainCharacter) = event.to;
}

void Engine::finalize(const WorldRepositionEvent& event)
{

}

PositionSystem* Engine::getPositionSystem()
{
    return &service::entity::ref().get<PositionSystem>();;
}
GraphicsSystem* Engine::getGraphicsSystem()
{
    return &service::entity::ref().get<GraphicsSystem>();
}

PhysicsSystem* Engine::getPhysicsSystem()
{
    return &service::entity::ref().get<PhysicsSystem>();
}
WorldStreamer* Engine::getWorldStreamer()
{
    return &service::entity::ref().get<WorldStreamer>();
}

void Engine::endGame()
{
    end = true;
}

Engine::~Engine()
{
    service::dispatcher::reset();
    service::resource::reset();
    service::entity::reset();
    service::input::reset();

}
