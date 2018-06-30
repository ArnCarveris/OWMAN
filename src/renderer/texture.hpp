#ifndef TEXTURE
#define TEXTURE

#include "low_level_renderer_2d.hpp"
#include "../math/aarect.hpp"
#include "../resource_manager/resource_manager.hpp"
#include "../resource_manager/resource_image.hpp"


class Texture;


class Texture
{
public:
    enum class Status
    {
        START,
        LOADING,
        LOADED,
        READY,
    };

    using Resource = core::resource::Data<ResourceImage, Texture, Status>;

    const static std::string texturesPath;

    typedef LowLevelRenderer2D::Texture::FilterMode FilterMode;


    Texture(){}

    /**
    * \brief set the filter mode
    */
    void setFilterMode(FilterMode filterMode);

    /**
    * \brief draw a texture in the screen
    */
    void draw
    (
        const Vec2f& pos,
        const Vec2f& scale,
        const LowLevelRenderer2D::SpriteVbo& sVbo
    )const;

    /**
    * \brief draw a texture in the screen
    */
    void draw
    (
        const Vec2f& pos,
        const LowLevelRenderer2D::SpriteVbo& sVbo
    )const;

    /**
    * \brief get the width of the texture
    * the texture must be ready
    */
    unsigned getWidth()const;

    /**
    * \brief get the height of the texture
    * the texture must be ready
    */
    unsigned getHeight()const;

private:
    FilterMode filterMode;

    LowLevelRenderer2D::Texture llTexture;
private:
    friend struct core::resource::LoaderProxy<Resource>;
};

#endif // TEXTURE
