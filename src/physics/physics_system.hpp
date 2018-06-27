#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM

#include <Box2D/Box2D.h>
#include "../math/vec2f.hpp"

#include "physics_component.hpp"

class PhysicsSystem
{
    b2World* world;

public:

    PhysicsSystem();

    void update(unsigned int delta);

    void destroyPhysicsComponent( PhysicsComponent* physicsComponent );

    b2World* getWorld();

    PhysicsComponent* createComponent(const Vec2f& position, xml_node<>* node, const bool kinematic);

    rapidxml::xml_node<>* createXmlNode(PhysicsComponent* component, rapidxml::xml_document<>* doc);

};

#endif

