#include "physics_system.hpp"

#include "../math/functions.hpp"
#include "../util/xmlstr.hpp"
#include <sstream>

using namespace std;
using namespace rapidxml;

PhysicsSystem::PhysicsSystem()
{

    b2Vec2 gravity(0, 0);
    world = new b2World(gravity);

}

void PhysicsSystem::update(unsigned int delta)
{

    float seconds = delta / 1000.0f;
    world->Step(seconds, 12, 5);

}

PhysicsComponent* PhysicsSystem::createPhysicsBox
    ( const Vec2f& position, const Vec2f& scale, float mass )
{

    PhysicsComponent* component = new PhysicsComponent;
    component->myPhysicsSystem = this;


    b2BodyDef bodyDef;
    bodyDef.type = (mass == 0) ? b2_staticBody : b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.active = true;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.linearDamping = 0.2;
    component->body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(scale.x / 2, scale.y / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.density = mass / (scale.x*scale.y);
    fixtureDef.restitution = 0.5;
    fixtureDef.shape = &shape;

    component->body->CreateFixture(&fixtureDef);

    return component;

}

PhysicsComponent* PhysicsSystem::createPhysicsBoxKinematic
    ( const Vec2f& position, const Vec2f& scale, float mass )
{

    PhysicsComponent* component = new PhysicsComponent;
    component->myPhysicsSystem = this;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.active = true;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.linearDamping = 0.01;
    component->body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(scale.x / 2, scale.y / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.density = mass / (scale.x*scale.y);
    fixtureDef.shape = &shape;

    component->body->CreateFixture(&fixtureDef);

    return component;

}

PhysicsComponent* PhysicsSystem::createPhysicsCircle
    ( const Vec2f& position, float radius, float mass )
{
    PhysicsComponent* component = new PhysicsComponent;
    component->myPhysicsSystem = this;

    b2BodyDef bodyDef;
    bodyDef.type = (mass == 0) ? b2_staticBody : b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.active = true;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.linearDamping = 0.2;
    component->body = world->CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.density = mass / (M_PI * radius*radius);
    fixtureDef.restitution = 0.5;
    fixtureDef.shape = &shape;

    component->body->CreateFixture(&fixtureDef);
    return component;
}

PhysicsComponent* PhysicsSystem::createPhysicsCircleKinematic
    ( const Vec2f& position, float radius, float mass )
{
    PhysicsComponent* component = new PhysicsComponent;
    component->myPhysicsSystem = this;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.active = true;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.linearDamping = 0.01;
    component->body = world->CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.density = mass / (M_PI * radius*radius);
    fixtureDef.shape = &shape;

    component->body->CreateFixture(&fixtureDef);

    return component;
}

// destroyers

void PhysicsSystem::destroyPhysicsComponent( PhysicsComponent* component)
{
    component->destroyDispatcher();
}

b2World* PhysicsSystem::getWorld()
{
    return world;
}

rapidxml::xml_node<>* PhysicsSystem::createXmlNode(PhysicsComponent * component, rapidxml::xml_document<>* doc)
{
    stringstream ss;
    char* s;
    xml_node<>* root = doc->allocate_node(node_element, xmlstr::physics);

    switch (component->body->GetFixtureList()->GetShape()->GetType())
    {
    case b2Shape::Type::e_circle: {

        // shape
        xml_node<>* shape_node = doc->allocate_node(node_element, xmlstr::shape, xmlstr::circle);
        root->append_node(shape_node);

        // radius
        ss << component->body->GetFixtureList()->GetShape()->m_radius;
        string radStr = ss.str();
        s = doc->allocate_string(radStr.c_str());
        xml_node<>* width_node = doc->allocate_node(node_element, xmlstr::radius, s);
        root->append_node(width_node);

    } break;

    case b2Shape::Type::e_polygon: {

        // shape
        xml_node<>* shape_node = doc->allocate_node(node_element, xmlstr::shape, xmlstr::box);
        root->append_node(shape_node);

        auto& aabb = component->body->GetFixtureList()->GetAABB(0);

        // width
        ss << int(aabb.upperBound.x - aabb.lowerBound.x);
        string width = ss.str();
        s = doc->allocate_string(width.c_str());
        xml_node<>* width_node = doc->allocate_node(node_element, xmlstr::width, s);
        root->append_node(width_node);

        ss.str(string());
        ss.clear();
        // height
        ss << int(aabb.upperBound.y - aabb.lowerBound.y);
        string height = ss.str();
        s = doc->allocate_string(height.c_str());
        xml_node<>* height_node = doc->allocate_node(node_element, xmlstr::height, s);
        root->append_node(height_node);
    } break;
    default:
        break;
    }

    ss.str(string());
    ss.clear();
    // mass
    ss << component->body->GetMass();
    string mass = ss.str();
    s = doc->allocate_string(mass.c_str());
    xml_node<>* mass_node = doc->allocate_node(node_element, xmlstr::mass, s);
    root->append_node(mass_node);

    return root;
}
