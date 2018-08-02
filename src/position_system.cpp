#include "position_system.hpp"
#include "util/xmlstr.hpp"
#include "world_streamer.hpp"
#include "entity.hpp"
#include "entity.hpp"
#include <string>
#include <iostream>
#include <sstream>

PositionSystem::PositionSystem(PositionSystem&& rhs) :
    center(std::move(rhs.center))
{
    auto& position_construction = service::entity::ref().construction<Position>();

    position_construction.disconnect<PositionSystem, &PositionSystem::createdComponent>(&rhs);
    position_construction.connect<PositionSystem, &PositionSystem::createdComponent>(this);
}

void PositionSystem::setRelativeCell(const Vec2i& cell)
{
    auto cellSize = service::entity::ref().get<WorldStreamer>().getCellSize();
    auto relativeCell = cell - service::entity::ref().get<WorldStreamer>().getWindowPosition();

    center.x = relativeCell.x * cellSize;
    center.y = relativeCell.y * cellSize;
}

void PositionSystem::recalc(const Vec2f& input, Position& output)
{
    float cellSize = service::entity::ref().get<WorldStreamer>().getCellSize();

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

