#ifndef PHYSICS_COMPONENT
#define PHYSICS_COMPONENT

#include "../math/vec2f.hpp"
#include <rapidxml.hpp>

class PhysicsSystem;
class b2Body;

class PhysicsComponent
{

    friend class PhysicsSystem;

protected:

    b2Body* body;

public:

    Vec2f getPosition()const;
    void setPosition(const Vec2f& v);

    Vec2f getSpeed()const;
    void setSpeed(const Vec2f& v);

    template <class Archive>
    void load(Archive & ar);

    template <class Archive>
    void save(Archive & ar) const;
};

#endif
