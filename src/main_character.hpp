
#include "math/vec2i.hpp"


#ifndef MAIN_CHARACTER
#define MAIN_CHARACTER

/**
 * The only difference with a normal entity is that
 * a main character stores the cell where it is located
 */
struct MainCharacter
{
    Vec2i cell;

    const Vec2i& getCell()const;
    Vec2i& getCell();
    void setCell( const Vec2i& cell );

};

#endif
