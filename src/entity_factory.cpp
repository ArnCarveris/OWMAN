#include "entity_factory.hpp"
#include "cell_size.hpp"
#include "engine.hpp"
#include "util/xmlstr.hpp"

#include "position_system.hpp"
#include "physics/physics_system.hpp"
#include "renderer/graphics_system.hpp"

using namespace rapidxml;
using namespace std;

EntityFactory::EntityFactory(Engine* engine)
{
	myEngine = engine;
}


Entity EntityFactory::createEntity(rapidxml::xml_node<> *node)
{
	Entity entity = registry.create();

	// position
	xml_node<> *position_node = node->first_node(xmlstr::position);
    if (position_node)
    {
        myEngine->getPositionSystem()->assignComponent(registry, entity, position_node);
    }

	// graphics
	xml_node<> *graphics_node = node->first_node(xmlstr::graphics);
	if( graphics_node )
	{
        myEngine->getGraphicsSystem()->assignComponent(registry, entity, graphics_node);
    }

	// physics
	xml_node<> *physics_node = node->first_node(xmlstr::physics);
	if( physics_node )
	{
        myEngine->getPhysicsSystem()->assignComponent(registry, entity, physics_node);
    }


	return entity;

}


rapidxml::xml_node<>* EntityFactory::createXmlNode(Entity entity, rapidxml::xml_document<>* doc)
{

    xml_node<>* node_ent;
    node_ent = doc->allocate_node(node_element, xmlstr::entity);


    /// position
    if (xml_node<>* position_node = myEngine->getPositionSystem()->createXmlNode(registry, entity, doc))
    {
        node_ent->append_node(position_node);
    }
    /// graphics component
    if (xml_node<>* graphics_node = myEngine->getGraphicsSystem()->createXmlNode(registry, entity, doc))
    {
        node_ent->append_node(graphics_node);
    }

    /// physics component
    if (xml_node<>* physics_node = myEngine->getPhysicsSystem()->createXmlNode(registry, entity, doc))
    {
        node_ent->append_node(physics_node);
    }

    return node_ent;

}

void EntityFactory::destroyEntity(Entity entity)
{
    registry.destroy(entity);
}
