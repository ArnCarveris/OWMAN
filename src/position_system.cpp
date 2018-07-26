#include "position_system.hpp"
#include "util/xmlstr.hpp"
#include "i_world_streamer.hpp"
#include "entity.hpp"
#include <string>
#include <iostream>
#include <sstream>

PositionSystem::PositionSystem() :
    center(0,0)
{
    service::entity::ref().construction<Position>().connect<PositionSystem, &PositionSystem::createdComponent>(this);
}
PositionSystem::~PositionSystem()
{
}

void PositionSystem::setRelativeCell(const Vec2i& cell)
{
    auto cellSize = service::world_streamer::ref().getCellSize();
    auto relativeCell = cell - service::world_streamer::ref().getWindowPosition();

    center.x = relativeCell.x * cellSize;
    center.y = relativeCell.y * cellSize;
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

void PositionSystem::update()
{
    service::entity::ref().view<Vec2f, Position>().each
    (
        [](const Entity entity, Vec2f& input, Position& output)
        {
            recalc(input, output);
        }
    );
}

void PositionSystem::createdComponent(EntityRegistry& registry, Entity entity)
{
    auto& component = registry.get<Position>(entity);

    registry.assign<Vec2f>(entity, center + component.getOffset());
}

