#include "main_character.hpp"

const Vec2i& MainCharacter::getCell()const
{

    return cell;

}

Vec2i& MainCharacter::getCell()
{

    return cell;

}

void MainCharacter::setCell( const Vec2i& cell )
{

    this->cell = cell;

}
