#pragma once

#include "position.hpp"

#include <rapidxml.hpp>
#include "entity.hpp"

class PositionSystem
{
public:
    PositionSystem();
    ~PositionSystem();
public:
    void setRelativeCell(const Vec2i& cell);

    void recalc(const Vec2f& input, Position& output);

    void assignComponent(EntityRegistry& registry, Entity entity, rapidxml::xml_node<>* node);

    void createdComponent(EntityRegistry& registry, Entity entity);

    rapidxml::xml_node<>* createXmlNode(EntityRegistry& registry, Entity entity, rapidxml::xml_document<>* doc);
private:
    Vec2i center;
};