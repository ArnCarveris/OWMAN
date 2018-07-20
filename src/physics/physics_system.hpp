#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM

#include <Box2D/Box2D.h>
#include "../math/vec2f.hpp"
#include "../entity.hpp"
#include "../events.hpp"
#include <rapidxml.hpp>

class PhysicsSystem
{
    b2World* world;

public:

    PhysicsSystem();

    void receive(const WorldRepositionEvent& event);

    void update(unsigned int delta);


    b2World* getWorld();

    void assignComponent(EntityRegistry& registry, Entity entity, rapidxml::xml_node<>* node);

    void createdComponent(EntityRegistry& registry, Entity entity);

    void destroyComponent(EntityRegistry& registry, Entity entity);

    rapidxml::xml_node<>* createXmlNode(EntityRegistry& registry, Entity entity, rapidxml::xml_document<>* doc);

};

#endif

