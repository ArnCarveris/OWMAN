#include "sprite_status.hpp"
#include "sprite.hpp"
#include "animation.hpp"
#include "animation_frame.hpp"
#include "../math/functions.hpp"
#include "graphics_system.hpp"
#include "texture.hpp"
#include "../util/xmlstr.hpp"
#include <iostream>

using namespace std;
using namespace rapidxml;

SpriteStatus::SpriteStatus(GraphicsSystem* system, const core::resource::Handle<Sprite::Resource>& sprite)
:
    GraphicsComponent(system),
    sprite(sprite)
{
    currentAnimation = 0;
    currentFrame = 0;
    elapsedTime = 0.0f;
    //cout << "-" << endl;
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

void SpriteStatus::draw()const
{
    if(isReady())
    {
        const Animation& anim = sprite->get().animations[currentAnimation];
        const AnimationFrame& frame = anim.frames[currentFrame];
        auto& texture = sprite->get().textures[frame.textureIndex]->get();

        const Vec2f& pos = position;
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

rapidxml::xml_node<>* SpriteStatus::createXmlNode(rapidxml::xml_document<>* doc)
{
    const char* spriteName = *sprite;

    xml_node<>* root = GraphicsComponent::createXmlNode(doc);
    const char* str_spriteName = doc->allocate_string(spriteName);
    xml_node<>* node_sprite = doc->allocate_node(node_element, xmlstr::sprite, str_spriteName);
    root->prepend_node(node_sprite);

    return root;
}

void SpriteStatus::destroyDispatcher()
{
    service::resource::ref().release(sprite);

    delete this;
}
