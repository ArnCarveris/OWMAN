#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM

#include <Box2D/Box2D.h>
#include "../math/vec2f.hpp"
#include "../entity.hpp"

#include "physics_component.hpp"

class PhysicsSystem
{
    b2World* world;

public:

    PhysicsSystem();

    void receive(const Vec2f::RepositionEvent<Entity>& event);

    void update(unsigned int delta);


    b2World* getWorld();

    void assignComponent(EntityRegistry& registry, Entity entity, rapidxml::xml_node<>* node, const bool kinematic);

    void destroyComponent(EntityRegistry& registry, Entity entity);

    rapidxml::xml_node<>* createXmlNode(EntityRegistry& registry, Entity entity, rapidxml::xml_document<>* doc);

};

#endif

