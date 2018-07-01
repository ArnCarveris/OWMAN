#ifndef SPRITE_STATUS
#define SPRITE_STATUS

#include "sprite.hpp"
#include "graphics_component.hpp"
#include <string>

class SpriteManager;

class SpriteStatus : public GraphicsComponent
{
    unsigned currentAnimation;
    unsigned currentFrame;
    float elapsedTime;  //< elapsed time in the current frame

    core::resource::Handle<Sprite::Resource> sprite;
public:

    SpriteStatus(GraphicsSystem* system, const core::resource::Handle<Sprite::Resource>& sprite);

    int getCurrentFrame()const;
    float getElapsedTime()const;

    void setCurrentFrame(int frame);
    void setElapsedTime(float time);

    bool isReady()const;
    void update(float delta);
    void draw()const;

    unsigned getAnimationIndex()const;
    void setAnimation(const std::string& animName);
    void setAnimation(unsigned animIndex);

    rapidxml::xml_node<>* createXmlNode(rapidxml::xml_document<>* doc);

    void destroyDispatcher();

};

#endif // SPRITE_STATUS
