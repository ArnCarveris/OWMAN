#ifndef SPRITE_STATUS
#define SPRITE_STATUS

#include "sprite.hpp"
#include <string>

class GraphicsSystem;

class SpriteStatus
{
    friend class GraphicsSystem;

    GraphicsSystem* myGraphicsSystem;
    bool visible;
    int priority;	// the higher the more priority

    Vec2f position;
    Vec2f scale;

    unsigned currentAnimation;
    unsigned currentFrame;
    float elapsedTime;  //< elapsed time in the current frame

    core::resource::Handle<Sprite::Resource> sprite;
public:

    SpriteStatus(GraphicsSystem* system, const core::resource::Handle<Sprite::Resource>& sprite);

    GraphicsSystem* getGraphicsSystem();
    
    bool isVisible()const;
    void setVisible(bool visible);


    const Vec2f& getPosition()const;
    Vec2f& getPosition();
    Vec2f getScale()const;
    void setPosition(const Vec2f& position);
    void setScale(const Vec2f& scale);
    int getPriority()const;
    void setPriority(int priority);

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

    void destroyDispatcher();

};

#endif // SPRITE_STATUS
