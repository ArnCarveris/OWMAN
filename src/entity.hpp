#include <string>
#include "position.hpp"
#include <rapidxml.hpp>

#ifndef SPRITE_STATUS
class SpriteStatus;
#endif

#ifndef PHYSICS_COMPONENT
class PhysicsComponent;
#endif

#ifndef ENTITY
#define ENTITY

/** \brief Components compounded entity
 *
 */
class Entity
{

public:

	typedef std::string TypeId;
	typedef unsigned int Id;

	friend class EntityFactory;

protected:

	TypeId typeId;
	Id id;

    SpriteStatus* graphicsComponent;
	PhysicsComponent* physicsComponent;

	Entity();

	void setGraphicsComponent(SpriteStatus* component);
	void setPhysicsComponent(PhysicsComponent* component);

public:

	TypeId getTypeOfEntity()const;
	Id getId()const;

	Vec2f getPosition()const;
	void setPosition(const Vec2f& pos);

    SpriteStatus* getGraphicsComponent();
	PhysicsComponent* getPhysicsComponent();


	~Entity();

};

#endif
