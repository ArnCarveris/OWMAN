#include "position_system.hpp"
#include "util/xmlstr.hpp"
#include "i_world_streamer.hpp"
#include <string>
#include <iostream>
#include <sstream>

PositionSystem::PositionSystem()
{
}
PositionSystem::~PositionSystem()
{
}

void PositionSystem::recalc(const Vec2f& input, Position& output)
{
    float cellSize = service::world_streamer::ref().getCellSize();

    float x = input.x;
    float y = input.y;
    // we have to do this because positions are wrt the cell
    // where the main character is and not wrt the cell where
    // this entity is
    while (x < 0) x += cellSize;
    while (y < 0) y += cellSize;


    output.setOffset(Vec2f(fmodf(x, cellSize), fmodf(y, cellSize)));
    output.setCell(Vec2i(x / cellSize, y / cellSize));
}

void PositionSystem::assignComponent(EntityRegistry& registry, Entity entity, rapidxml::xml_node<>* node, const Vec2i& toCenter)
{
    float cellSize = service::world_streamer::ref().getCellSize();

    auto& component = registry.assign<Position>(entity);

    Vec2f& pos = registry.assign<Vec2f>(entity, toCenter.x * cellSize, toCenter.y * cellSize);

    rapidxml::xml_node<> *x_position_node = node->first_node(xmlstr::x);
    rapidxml::xml_node<> *y_position_node = node->first_node(xmlstr::y);

    rapidxml::xml_node<> *cell_x_node = node->first_node(xmlstr::cell_x);
    rapidxml::xml_node<> *cell_y_node = node->first_node(xmlstr::cell_y);

    if (x_position_node && y_position_node)
    {
        component.setOffset(Vec2i(atof(x_position_node->value()), atof(y_position_node->value())));

        pos += component.getOffset();
    }

    if (cell_x_node && cell_y_node)
    {
        component.setCell(Vec2i(atoi(cell_x_node->value()), atoi(cell_y_node->value())));
    }
}


rapidxml::xml_node<>* PositionSystem::createXmlNode(EntityRegistry& registry, Entity entity, rapidxml::xml_document<>* doc)
{
    if (!registry.has<Position>(entity))
    {
        return nullptr;
    }
    char* s;
    std::stringstream ss;

    auto& component = registry.get<Position>(entity);

    rapidxml::xml_node<>* pos_node = doc->allocate_node(rapidxml::node_element, xmlstr::position);

#ifdef NOT_USED
    /// cell_x
    {
        ss.str(std::string());
        ss.clear();
        ss << component.getCell().x;
        std::string sx = ss.str();
        char* s = doc->allocate_string(sx.c_str());
        rapidxml::xml_node<>* x_node = doc->allocate_node(rapidxml::node_element, xmlstr::cell_x, s);
        pos_node->append_node(x_node);
    }

    /// cell_y
    {
        ss.str(std::string());
        ss.clear();
        ss << component.getCell().y;
        std::string sy = ss.str();
        s = doc->allocate_string(sy.c_str());
        rapidxml::xml_node<>* y_node = doc->allocate_node(rapidxml::node_element, xmlstr::cell_y, s);
        pos_node->append_node(y_node);
    }
#else
    recalc(registry.get<Vec2f>(entity), component);
#endif
    /// x
    {
        ss.str(std::string());
        ss.clear();
        ss << component.getOffset().x;
        std::string sx = ss.str();
        s = doc->allocate_string(sx.c_str());
        rapidxml::xml_node<>* x_node = doc->allocate_node(rapidxml::node_element, xmlstr::x, s);
        pos_node->append_node(x_node);
    }

    /// y
    {
        ss.str(std::string());
        ss.clear();
        ss << component.getOffset().y;
        std::string sy = ss.str();
        s = doc->allocate_string(sy.c_str());
        rapidxml::xml_node<>* y_node = doc->allocate_node(rapidxml::node_element, xmlstr::y, s);
        pos_node->append_node(y_node); 
    }

    return pos_node;

}