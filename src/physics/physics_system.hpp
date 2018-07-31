#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM

#include <Box2D/Box2D.h>
#include "../math/vec2f.hpp"
#include "../entity.hpp"
#include "../events.hpp"

class PhysicsSystem
{
    b2World* world;

public:

    PhysicsSystem();

    void receive(const WorldRepositionEvent& event);

    void update(unsigned int delta);


    b2World* getWorld();

 
    void createdComponent(EntityRegistry& registry, Entity entity);

    void destroyComponent(EntityRegistry& registry, Entity entity);


    template<typename Archive>
    void serialize(Archive&);
};

#endif

