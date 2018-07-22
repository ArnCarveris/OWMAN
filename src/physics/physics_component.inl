#include "../math/vec2f.hpp"
#include "../math/functions.hpp"
#include <math.h>
#include <string>
#include <cereal/cereal.hpp>
#include "../engine.hpp"
#include "physics_system.hpp"

template <class Archive>
void PhysicsComponent::load(Archive & ar)
{
    float mass;
    std::string shape;
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;

    bool kinematic = false;
    try
    {
        ar(cereal::make_nvp(xmlstr::kinematic, kinematic));
    }
    catch (const std::exception e) { (void)e; }

    ar(cereal::make_nvp(xmlstr::mass, mass));
    ar(cereal::make_nvp(xmlstr::shape, shape));


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

    body = service::engine::ref().getPhysicsSystem()->getWorld()->CreateBody(&bodyDef);


    if (shape == xmlstr::box)
    {
        Vec2f scale;

        ar(cereal::make_nvp(xmlstr::width, scale.x));
        ar(cereal::make_nvp(xmlstr::height, scale.y));

        b2PolygonShape shape;

        shape.SetAsBox(scale.x / 2, scale.y / 2);

        fixtureDef.density = mass / (scale.x*scale.y);
        fixtureDef.shape = &shape;

        body->CreateFixture(&fixtureDef);
    }
    else if (shape == xmlstr::circle)
    {
        float radius; ar(cereal::make_nvp(xmlstr::radius, radius));

        b2CircleShape shape;

        shape.m_radius = radius;

        fixtureDef.density = mass / (M_PI * radius*radius);
        fixtureDef.shape = &shape;

        body->CreateFixture(&fixtureDef);
    }
}

template <class Archive>
void PhysicsComponent::save(Archive & ar) const
{
    ar(cereal::make_nvp(xmlstr::mass, body->GetMass()));

    switch (body->GetFixtureList()->GetShape()->GetType())
    {
    case b2Shape::Type::e_circle: {
        ar(cereal::make_nvp(xmlstr::shape, std::string{ xmlstr::circle }));
        ar(cereal::make_nvp(xmlstr::radius, body->GetFixtureList()->GetShape()->m_radius));
    } break;

    case b2Shape::Type::e_polygon: {
        auto& aabb = body->GetFixtureList()->GetAABB(0);

        ar(cereal::make_nvp(xmlstr::shape, std::string{ xmlstr::box }));
        ar(cereal::make_nvp(xmlstr::width, floorf(aabb.upperBound.x - aabb.lowerBound.x)));
        ar(cereal::make_nvp(xmlstr::height, floorf(aabb.upperBound.y - aabb.lowerBound.y)));
    } break;
    default:
        break;
    }
}