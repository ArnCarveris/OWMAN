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
        entity->setGraphicsComponent(myEngine->getGraphicsSystem()->createComponent(graphics_node));
    }

	// physics
	xml_node<> *physics_node = node->first_node(xmlstr::physics);
	if( physics_node )
	{
        entity->setPhysicsComponent(myEngine->getPhysicsSystem()->createComponent(pos, physics_node, false));
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
        entity->setGraphicsComponent(myEngine->getGraphicsSystem()->createComponent(graphics_node));
    }

	// physics
    xml_node<> *physics_node = node->first_node(xmlstr::physics);

    if( physics_node )
    {
        entity->setPhysicsComponent(myEngine->getPhysicsSystem()->createComponent(pos, physics_node, true));
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
        myEngine->getPhysicsSystem()->destroyComponent(pc);
    }

}
