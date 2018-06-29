#ifndef TEXTURE
#define TEXTURE

#include "low_level_renderer_2d.hpp"
#include "../math/aarect.hpp"
#include "../resource_manager/resource_manager.hpp"
#include "../resource_manager/resource_texture.hpp"


class Texture;

using texture = core::resource::Data<ResourceTexture, Texture>;

class Texture
{
    friend struct core::resource::LoaderProxy<texture>;
public:

    const static std::string texturesPath;

    typedef LowLevelRenderer2D::Texture::FilterMode FilterMode;

    enum class Status
    {
        START,
        LOADING,
        LOADED,
        READY,
    };

    Texture(){}


    /**
    * \brief get the name of the texture file
    */
    const std::string& getName()const;

    /**
    * \brief set the filter mode
    */
    void setFilterMode(FilterMode filterMode);

    /**
    * \brief is loaded in main memory
    */
    bool isLoaded()const;

    /**
    * \brief is laoded in graphics card
    */
    bool isReady()const;

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
    std::string name;
    Status status;
    FilterMode filterMode;

    LowLevelRenderer2D::Texture llTexture;

};

#endif // TEXTURE
