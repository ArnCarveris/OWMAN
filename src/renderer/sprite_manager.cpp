#include "sprite_manager.hpp"
#include "resource_manager.hpp"
#include <cassert>

SpriteStatus* SpriteManager::instanceSprite(const std::string name)
{
    auto it = sprites.find(name);
    Sprite* sprite;

    // if is already instanced
    if(it != sprites.end())
    {
        sprite = it->second.sprite;
    }

    // sprite not instanced, need to create
    else{
        sprite = new Sprite(this);
    }

    // increment reference count to the sprite
    it->second.count++;

    SpriteStatus* spriteStatus = new SpriteStatus(sprite);
    return spriteStatus;

}

void SpriteManager::releaseSpriteInstance(SpriteStatus* spriteStatus)
{
    auto it = sprites.find(spriteStatus->sprite->getName());

    assert
    (
        it != sprites.end() &&
        "Attempt to release a sprite status whose sprite is not in the table"
    );

    SpriteRefCountEntry& entry = it->second;
    if(entry.count == 1)
    {
        sprites.erase(it);
    }
    else
    {
        entry.count--;
    }

}

void SpriteManager::update(float delta)
{
    // 1: check if resoruces have been laoded

    // 2: handle animations

}