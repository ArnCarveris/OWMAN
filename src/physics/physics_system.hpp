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

    /** \brief Create a dynamic box
     * Dynamic means that it is affected by forces
     */
    PhysicsComponent* createPhysicsBox( const Vec2f& position, const Vec2f& scale, float mass );

    /** \brief Create a kinematic box
     * Kinematic means that it is not affected by forces
     */
    PhysicsComponent* createPhysicsBoxKinematic( const Vec2f& position, const Vec2f& scale, float mass );

    /** \brief Create a dynamic circle
    * Dynamic means that it is affected by forces
    */
    PhysicsComponent* createPhysicsCircle( const Vec2f& position, float radius, float mass );

    /** \brief Create a kinematic circle
    * Kinematic means that it is not affected by forces
    */
    PhysicsComponent* createPhysicsCircleKinematic( const Vec2f& position, float radius, float mass );

    void destroyPhysicsComponent( PhysicsComponent* physicsComponent );

    b2World* getWorld();

    rapidxml::xml_node<>* createXmlNode(PhysicsComponent* component, rapidxml::xml_document<>* doc);

};

#endif

