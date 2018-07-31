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

    static void recalc(const Vec2f& input, Position& output);

    void update();

    void createdComponent(EntityRegistry& registry, Entity entity);

    template<typename Archive>
    void serialize(Archive&);
private:
    Vec2i center;
};