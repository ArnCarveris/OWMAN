#include "sprite_status.hpp"
#include "sprite.hpp"
#include "animation.hpp"
#include "animation_frame.hpp"
#include "../math/functions.hpp"
#include "texture.hpp"
#include "../util/xmlstr.hpp"
#include <iostream>

using namespace std;
using namespace rapidxml;

SpriteStatus::SpriteStatus(const core::resource::Handle<Sprite::Resource>& sprite)
:
    sprite(sprite)
{
    //cout << "-" << endl;
}


bool SpriteStatus::isVisible()const
{
    return visible;
}

void SpriteStatus::setVisible(bool visible)
{
    this->visible = visible;
}


const Vec2f& SpriteStatus::getScale()const
{
    return scale;
}


void SpriteStatus::setScale(const Vec2f& scale)
{
    this->scale = scale;
}

int SpriteStatus::getPriority()const
{
    return priority;
}

void SpriteStatus::setPriority(int priority)
{
    this->priority = priority;
}

int SpriteStatus::getCurrentFrame()const
{
    return currentFrame;
}

float SpriteStatus::getElapsedTime()const
{
    return elapsedTime;
}

void SpriteStatus::setCurrentFrame(int frame)
{
    currentFrame = frame;
}

void SpriteStatus::setElapsedTime(float time)
{
    elapsedTime = time;
}


bool SpriteStatus::isReady()const
{
    return sprite && sprite->get_status() == Sprite::Status::EVERYTHING_LOADED;
}

void SpriteStatus::update(float delta)
{
    if(isReady())
    {
        elapsedTime += delta;
        const Animation& anim = sprite->get().animations[currentAnimation];
        const AnimationFrame& frame = anim.frames[currentFrame];
        float frameDuration = frame.frameDuration;
        if(elapsedTime >= frameDuration)
        {
            unsigned incf = (unsigned)(elapsedTime/frameDuration);
            elapsedTime = fmod(elapsedTime, frameDuration);
            currentFrame += incf;
            currentFrame %= anim.frames.size();
        }
    }
}

void SpriteStatus::draw(const Vec2f& pos)const
{
    if(isReady())
    {
        const Animation& anim = sprite->get().animations[currentAnimation];
        const AnimationFrame& frame = anim.frames[currentFrame];
        auto& texture = sprite->get().textures[frame.textureIndex]->get();

        const Vec2f& scale = this->scale;
        const LowLevelRenderer2D::SpriteVbo& vbo = frame.vbo;

        texture.draw(pos, scale, vbo);
    }
}

unsigned SpriteStatus::getAnimationIndex()const
{
    return currentAnimation;
}

void SpriteStatus::setAnimation(const string& animName)
{
    unsigned index = sprite->get().getAnimIndex(animName);
    if(index != currentAnimation)
    {
        currentAnimation = index;
        currentFrame = 0;
    }
}

void SpriteStatus::setAnimation(unsigned animIndex)
{
    if(animIndex != currentAnimation)
    {
        currentAnimation = animIndex;
        currentFrame = 0;
    }
}


