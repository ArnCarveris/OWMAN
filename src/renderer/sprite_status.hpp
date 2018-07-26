#ifndef SPRITE_STATUS
#define SPRITE_STATUS

#include "sprite.hpp"
#include <string>
#include "../util/xmlstr.hpp"
#include <cereal/cereal.hpp>

class GraphicsSystem;

class SpriteStatus
{
    friend class GraphicsSystem;
    bool visible{ true };
    int priority{ 0 };	// the higher the more priority

    Vec2f scale{ 1, 1 };

    unsigned currentAnimation{ 0 };
    unsigned currentFrame{ 0 };
    float elapsedTime{ 0.0f };  //< elapsed time in the current frame

    core::resource::Handle<Sprite::Resource> sprite;
public:
    SpriteStatus() = default;
    SpriteStatus(SpriteStatus&&) = default;
    SpriteStatus(const SpriteStatus&) = default;

    SpriteStatus& operator=(SpriteStatus&&) = default;
    SpriteStatus& operator=(const SpriteStatus&) = default;

    bool isVisible()const;
    void setVisible(bool visible);


    const Vec2f& getScale()const;
    void setScale(const Vec2f& scale);
    int getPriority()const;
    void setPriority(int priority);

    int getCurrentFrame()const;
    float getElapsedTime()const;

    void setCurrentFrame(int frame);
    void setElapsedTime(float time);

    bool isReady()const;
    void update(float delta);
    void draw(const Vec2f& pos)const;

    unsigned getAnimationIndex()const;
    void setAnimation(const std::string& animName);
    void setAnimation(unsigned animIndex);

    template<typename Archive>
    void serialize(Archive &archive);
};

#endif // SPRITE_STATUS
