#include "physics_system.hpp"
#include "../dispatcher.hpp"
#include "../entity_factory.hpp"
#include "../math/functions.hpp"
#include "../util/xmlstr.hpp"
#include <sstream>

using namespace std;
using namespace rapidxml;

PhysicsSystem::PhysicsSystem()
{

    b2Vec2 gravity(0, 0);
    world = new b2World(gravity);

    service::entity::ref().registry.construction<PhysicsComponent>().connect<PhysicsSystem, &PhysicsSystem::createdComponent>(this);
    service::entity::ref().registry.destruction<PhysicsComponent>().connect<PhysicsSystem, &PhysicsSystem::destroyComponent>(this);
    service::dispatcher::ref().sink<Vec2f::RepositionEvent<Entity>>().connect(this);
}

void PhysicsSystem::receive(const WorldRepositionEvent& event)
{
    service::entity::ref().registry.view<PhysicsComponent, Vec2f>().each([](const Entity entity, PhysicsComponent& component, Vec2f& position) {
        component.setPosition(position);
    });
}

void PhysicsSystem::update(unsigned int delta)
{

    float seconds = delta / 1000.0f;
    world->Step(seconds, 12, 5);
    
    service::entity::ref().registry.view<PhysicsComponent, Vec2f>().each([](const Entity entity, PhysicsComponent& component, Vec2f& position) {
        position = component.getPosition();
    });
}

b2World* PhysicsSystem::getWorld()
{
    return world;
}

void PhysicsSystem::assignComponent(EntityRegistry& registry, Entity entity, xml_node<>* node)
{ 
    PhysicsComponent component;
    
    bool kinematic = false;

    if (auto *kinematic_node = node->first_node(xmlstr::kinematic))
    {
        if (auto* value = kinematic_node->value())
        {
            kinematic = !strcmp(value, "1") || !strcmpi(value, "true");
        }
    }

    xml_node<> *shape_node = node->first_node(xmlstr::shape);
    string sShape(shape_node->value());

    xml_node<> *mass_node = node->first_node(xmlstr::mass);
    float mass = atof(mass_node->value());

    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;

    bodyDef.fixedRotation = true;
    bodyDef.active = true;

    if (kinematic)
    {
        bodyDef.type = b2_dynamicBody;
        bodyDef.linearDamping = 0.01;
    }
    else
    {
        bodyDef.type = (mass == 0) ? b2_staticBody : b2_dynamicBody;
        bodyDef.linearDamping = 0.2;

        fixtureDef.restitution = 0.5;
    }

    component.body = world->CreateBody(&bodyDef);

    if (sShape == xmlstr::box)
    {
        xml_node<> *width_node = node->first_node(xmlstr::width);
        xml_node<> *height_node = node->first_node(xmlstr::height);

        Vec2f scale(atof(width_node->value()), atof(height_node->value()));

        b2PolygonShape shape;

        shape.SetAsBox(scale.x / 2, scale.y / 2);

        fixtureDef.density = mass / (scale.x*scale.y);
        fixtureDef.shape = &shape;

        component.body->CreateFixture(&fixtureDef);
    }
    else if (sShape == xmlstr::circle)
    {
        xml_node<> *radius_node = node->first_node(xmlstr::radius);
        float radius = atof(radius_node->value());

        b2CircleShape shape;

        shape.m_radius = radius;

        fixtureDef.density = mass / (M_PI * radius*radius);
        fixtureDef.shape = &shape;

        component.body->CreateFixture(&fixtureDef);
    }
    
    registry.assign<PhysicsComponent>(entity, std::move(component));
}

void PhysicsSystem::createdComponent(EntityRegistry& registry, Entity entity)
{
    auto& position = registry.get<Vec2f>(entity);
    auto& component = registry.get<PhysicsComponent>(entity);

    component.setPosition(position);
}

void PhysicsSystem::destroyComponent(EntityRegistry& registry, Entity entity)
{
    world->DestroyBody(registry.get<PhysicsComponent>(entity).body);
}

rapidxml::xml_node<>* PhysicsSystem::createXmlNode(EntityRegistry& registry, Entity entity, rapidxml::xml_document<>* doc)
{
    if (!registry.has<PhysicsComponent>(entity))
    {
        return nullptr;
    }

    auto& component = registry.get<PhysicsComponent>(entity);

    stringstream ss;
    char* s;
    xml_node<>* root = doc->allocate_node(node_element, xmlstr::physics);

    switch (component.body->GetFixtureList()->GetShape()->GetType())
    {
    case b2Shape::Type::e_circle: {

        // shape
        xml_node<>* shape_node = doc->allocate_node(node_element, xmlstr::shape, xmlstr::circle);
        root->append_node(shape_node);

        // radius
        ss << component.body->GetFixtureList()->GetShape()->m_radius;
        string radStr = ss.str();
        s = doc->allocate_string(radStr.c_str());
        xml_node<>* width_node = doc->allocate_node(node_element, xmlstr::radius, s);
        root->append_node(width_node);

    } break;

    case b2Shape::Type::e_polygon: {

        // shape
        xml_node<>* shape_node = doc->allocate_node(node_element, xmlstr::shape, xmlstr::box);
        root->append_node(shape_node);

        auto& aabb = component.body->GetFixtureList()->GetAABB(0);

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
    ss << component.body->GetMass();
    string mass = ss.str();
    s = doc->allocate_string(mass.c_str());
    xml_node<>* mass_node = doc->allocate_node(node_element, xmlstr::mass, s);
    root->append_node(mass_node);

    return root;
}
