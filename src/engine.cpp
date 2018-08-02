#include "engine.hpp"
#include "events.hpp"
#include "resource_manager/resource_manager.hpp"
#include "renderer/texture.hpp"
#include "renderer/sprite.hpp"
#include "IO/event_handler.hpp"

#include <rapidxml.hpp>
#include "util/file_to_string.hpp"
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>

#include "systems.hpp"
#include "systems.inl"

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
            service::dispatcher::ref().sink<WorldRepositionEvent>().connect<Engine, &Engine::finalize>(this);
            {
                auto entity = service::entity::ref().create();
                
                input(cereal::make_nvp("systems", service::entity::ref().assign<Systems>(entt::tag_t{}, entity)));
            }
            service::dispatcher::ref().sink<WorldRepositionEvent>().connect<Engine, &Engine::prepare>(this);
        } {
            input(cereal::make_nvp("world_entities", core::serialization::ResourcesMediator<WorldEntity::Resource>{ world_entities }));
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
    EntityRegistry& registry = service::entity::ref();
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
        registry.get<PhysicsSystem>().update(dt);
        registry.get<PositionSystem>().update();

        // update world streamer

        registry.get<WorldStreamer>().update(registry.get<Vec2f>(mainCharacter));

        // follow main character with the camera
        registry.get<GraphicsSystem>().getCamera()->setPosition(-registry.get<Vec2f>(mainCharacter));

        // update graphics
        registry.get<GraphicsSystem>().update(dt);

        // draw
        registry.get<GraphicsSystem>().draw();
        registry.get<GraphicsSystem>().swap();
    };
    {
        while (!end)
        {
            frame_start();
            service::resource::ref().synchronize();
            
            end = true;
            
            for (auto& res : world_entities)
            {
                if (res->get_status() != WorldEntity::Status::LOADED)
                {
                    end = false; break;
                }
            }

            frame_sleep();
        }

        end = false;
        mainCharacter = registry.attachee<MainCharacter>();

        registry.get<WorldStreamer>().init(registry.get<Position>(mainCharacter));
    }
    
    while( !end )
    {
        frame_start();

        service::resource::ref().synchronize();

        frame_tick(ticks - prevTicks);

        frame_sleep();
    } 

    {
        registry
            .get<Position>(mainCharacter)
            .setCell(registry.get<WorldStreamer>().getWindowPosition());

        for (auto& res : world_entities)
        {
            service::resource::ref().release(res);
        }
       
        registry.get<WorldStreamer>().end();
        service::resource::ref().stop();
    }

    while (end)
    {
        frame_start();
        end = service::resource::ref().synchronize();
        frame_sleep();
    }

    registry.get<GraphicsSystem>().end();
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
