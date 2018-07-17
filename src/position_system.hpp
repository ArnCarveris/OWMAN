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

    void recalc(const Vec2f& input, Position& output);

    void assignComponent(EntityRegistry& registry, Entity entity, rapidxml::xml_node<>* node, const Vec2i& toCenter);

    void destroyComponent(EntityRegistry& registry, Entity entity);

    rapidxml::xml_node<>* createXmlNode(EntityRegistry& registry, Entity entity, rapidxml::xml_document<>* doc);
};