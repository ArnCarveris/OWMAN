#pragma once

#include "entity.hpp"
#include "dispatcher.hpp"

#include "math/vec2f.hpp"
#include "math/vec2i.hpp"


using WorldRepositionEvent = Vec2f::RepositionEvent<Entity>;

struct EntityMigrationEvent
{
    Vec2i::RepositionEvent<Entity> cell;
    Entity entity;
};