#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM

#include <Box2D/Box2D.h>
#include "../math/vec2f.hpp"
#include "../entity.hpp"
#include "../events.hpp"

class PhysicsSystem
{
    std::unique_ptr<b2World> world;

public:

    PhysicsSystem();
    PhysicsSystem(const PhysicsSystem&) = delete;
    PhysicsSystem(PhysicsSystem&&);
    ~PhysicsSystem() = default;

    void receive(const WorldRepositionEvent& event);

    void update(unsigned int delta);


    b2World* getWorld();

 
    void createdComponent(EntityRegistry& registry, Entity entity);

    void destroyComponent(EntityRegistry& registry, Entity entity);


    template<typename Archive>
    void serialize(Archive&);
};

#endif

