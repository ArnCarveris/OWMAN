#include "entity_factory.hpp"
#include "cell_size.hpp"
#include "renderer/graphics_component.hpp"
#include "physics/physics_component.hpp"
#include "engine.hpp"
#include "renderer/sprite_status.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include "util/xmlstr.hpp"

using namespace rapidxml;
using namespace std;

EntityFactory::EntityFactory(Engine* engine)
{
	countId = DEFAULT_INIT_ID;
	myEngine = engine;
}


Entity* EntityFactory::createEntity
(
	xml_node<> *node,
	const Vec2i& toCenter
)
{

	Entity* entity = new Entity;

	Vec2f pos;
	pos.x = toCenter.x * myEngine->getCellSize();
	pos.y = toCenter.y * myEngine->getCellSize();

	xml_node<> *id_node = node->first_node("id");
	if( id_node != 0 )
	{
		entity->id = atoi(id_node->value());
	}
	else
	{
		entity->id = countId;
		countId++;
	}

	// position
	xml_node<> *position_node = node->first_node(xmlstr::position);
		xml_node<> *x_position_node = position_node->first_node(xmlstr::x);
		pos.x += atof( x_position_node->value() );
		xml_node<> *y_position_node = position_node->first_node(xmlstr::y);
		pos.y += atof( y_position_node->value() );

	// graphics
	xml_node<> *graphics_node = node->first_node(xmlstr::graphics);
	if( graphics_node )
	{

		xml_node<> *sprite_node = graphics_node->first_node(xmlstr::sprite);
		string spriteName = sprite_node->value();
		xml_node<> *width_graphics_node = graphics_node->first_node(xmlstr::width);
		float width_graphics = atof( width_graphics_node->value() );
		xml_node<> *height_graphics_node = graphics_node->first_node(xmlstr::height);
		float height_graphics = atof( height_graphics_node->value() );
		xml_node<> *priority_node = graphics_node->first_node(xmlstr::priority);

		GraphicsComponent* graphicsComponent =
        myEngine->getGraphicsSystem()->instanceSprite
        (
            spriteName,
            Vec2f( width_graphics, height_graphics )
        );

        if( priority_node )
		{
            int priority = atoi( priority_node->value() );
            graphicsComponent->setPriority( priority );
		}

        entity->setGraphicsComponent( graphicsComponent );

    }

	// physics
	xml_node<> *physics_node = node->first_node(xmlstr::physics);
	if( physics_node )
	{
        PhysicsComponent* physicsComponent;

        xml_node<> *shape_node = physics_node->first_node(xmlstr::shape);
        string sShape( shape_node->value() );

        xml_node<> *mass_node = physics_node->first_node(xmlstr::mass);
        float mass = atof( mass_node->value() );

        if(sShape == xmlstr::box)
        {
            xml_node<> *width_physics_node = physics_node->first_node(xmlstr::width);
            float width_physics = atof( width_physics_node->value() );
            xml_node<> *height_physics_node = physics_node->first_node(xmlstr::height);
            float height_physics = atof( height_physics_node->value() );

            physicsComponent =
            myEngine->getPhysicsSystem()->createPhysicsBox
            (
                pos,
                Vec2f(width_physics, height_physics),
                mass
            );
        }
        else if(sShape == xmlstr::circle)
        {
            xml_node<> *radius_node = physics_node->first_node(xmlstr::radius);
            float radius = atof( radius_node->value() );
            physicsComponent =
            myEngine->getPhysicsSystem()->createPhysicsCircle
            (
                pos,
                radius,
                mass
            );
        }
        else
        {cerr << "error: type of shape not recognized" << endl;}

        entity->setPhysicsComponent( physicsComponent );

    }


	entity->setPosition( pos );

	return entity;

}

MainCharacter* EntityFactory::createMainCharacter(rapidxml::xml_node<> *node)
{

    MainCharacter* entity = new MainCharacter;

	Vec2f pos;

	xml_node<> *id_node = node->first_node("id");
	if( id_node != 0 )
	{
		entity->id = atoi(id_node->value());
	}
	else
	{
		entity->id = countId;
		countId++;
	}

	// position
	xml_node<> *position_node = node->first_node(xmlstr::position);
        xml_node<> *cell_x_node = position_node->first_node(xmlstr::cell_x);
        int cell_x = atoi( cell_x_node->value() );
        xml_node<> *cell_y_node = position_node->first_node(xmlstr::cell_y);
        int cell_y = atoi( cell_y_node->value() );
		xml_node<> *x_position_node = position_node->first_node(xmlstr::x);
		pos.x = atof( x_position_node->value() );
		xml_node<> *y_position_node = position_node->first_node(xmlstr::y);
		pos.y = atof( y_position_node->value() );

	// graphics
	xml_node<> *graphics_node = node->first_node(xmlstr::graphics);
	if( graphics_node )
	{

		xml_node<> *sprite_node = graphics_node->first_node(xmlstr::sprite);
		string spriteName = sprite_node->value();
		xml_node<> *width_graphics_node = graphics_node->first_node(xmlstr::width);
		float width_graphics = atof( width_graphics_node->value() );
		xml_node<> *height_graphics_node = graphics_node->first_node(xmlstr::height);
		float height_graphics = atof( height_graphics_node->value() );
		xml_node<> *priority_node = graphics_node->first_node(xmlstr::priority);

		GraphicsComponent* graphicsComponent =
        myEngine->getGraphicsSystem()->instanceSprite
        (
            spriteName,
            Vec2f( width_graphics, height_graphics )
        );

        if( priority_node )
		{
            int priority = atoi( priority_node->value() );
            graphicsComponent->setPriority( priority );
		}

        entity->setGraphicsComponent( graphicsComponent );

    }

	// physics
    xml_node<> *physics_node = node->first_node(xmlstr::physics);

    if( physics_node )
    {

        xml_node<> *shape_node = physics_node->first_node(xmlstr::shape);
        string sShape( shape_node->value() );
        xml_node<> *width_physics_node = physics_node->first_node(xmlstr::width);
        float width_physics = atof( width_physics_node->value() );
        xml_node<> *height_physics_node = physics_node->first_node(xmlstr::height);
        float height_physics = atof( height_physics_node->value() );
        xml_node<> *mass_node = physics_node->first_node(xmlstr::mass);
        float mass = atof( mass_node->value() );

        PhysicsComponent* physicsComponent =
        myEngine->getPhysicsSystem()->createPhysicsBoxKinematic
        (
            pos,
            Vec2f(width_physics, height_physics),
            mass
        );

        entity->setPhysicsComponent( physicsComponent );

    }


	entity->setPosition( pos );
	entity->setCell( Vec2i(cell_x, cell_y) );

	return entity;

}

rapidxml::xml_node<>* EntityFactory::createXmlNode(Entity * entity, rapidxml::xml_document<>* doc, float cellSize)
{

    xml_node<>* node_ent;
    node_ent = doc->allocate_node(node_element, xmlstr::entity);


    float x = entity->getPosition().x;
    float y = entity->getPosition().y;
    // we have to do this because positions are wrt the cell
    // where the main character is and not wrt the cell where
    // this entity is
    while (x < 0) x += cellSize;
    while (x >= cellSize) x -= cellSize;
    while (y < 0) y += cellSize;
    while (y >= cellSize) y -= cellSize;

    xml_node<>* pos_node = doc->allocate_node(node_element, xmlstr::position);
    node_ent->append_node(pos_node);

    /// x
    stringstream ss;
    ss << x;
    string sx = ss.str();
    char* s = doc->allocate_string(sx.c_str());
    xml_node<>* x_node = doc->allocate_node(node_element, xmlstr::x, s);
    pos_node->append_node(x_node);

    /// y
    ss.str(string());
    ss.clear();
    ss << y;
    string sy = ss.str();
    s = doc->allocate_string(sy.c_str());
    xml_node<>* y_node = doc->allocate_node(node_element, xmlstr::y, s);
    pos_node->append_node(y_node);

    /// graphics component
    if (auto* component = entity->getGraphicsComponent())
    {
        xml_node<>* graphics_node = component->createXmlNode(doc);
        node_ent->append_node(graphics_node);
    }

    /// physics component
    if (auto* component = entity->getPhysicsComponent())
    {
        xml_node<>* physics_node = myEngine->getPhysicsSystem()->createXmlNode(component, doc);
        node_ent->append_node(physics_node);
    }

    return node_ent;

}

void EntityFactory::destroyEntity(Entity* entity)
{

    GraphicsComponent* gc = entity->getGraphicsComponent();
    if( gc )
    {
        myEngine->getGraphicsSystem()->destroyGraphicsComponent( gc );
    }

	PhysicsComponent* pc = entity->getPhysicsComponent();
	if( pc )
	{
        myEngine->getPhysicsSystem()->destroyPhysicsComponent(pc);
    }

}
