#include "texture.hpp"
#include "texture_manager.hpp"
#include <cassert>
#include "../resource_manager/resource_manager.hpp"
#include "../resource_manager/resource_texture.hpp"
#include "graphics_system.hpp"

const std::string Texture::texturesPath = "textures";

Texture::Texture(TextureManager* textureManager, const std::string& name)
{
    this->textureManager = textureManager;
    this->name = name;
    status = Status::START;
    llTexture = LowLevelRenderer2D::Texture();
    filterMode = FilterMode::NEAREST;
}

const std::string& Texture::getName()const
{
    return name;
}

void Texture::setFilterMode(FilterMode filterMode)
{
    this->filterMode = filterMode;
}

bool Texture::isLoaded()const
{
    return Status::LOADED == status;
}

bool Texture::isReady()const
{
    return Status::READY == status;
}

void Texture::loadResource()
{
    assert(!resourceHandle);
    auto id = texturesPath + "/" + name;
    resourceHandle = service::resource::ref().obtain<ResourceTexture>(core::resource::ID{id.c_str()});
    status = Status::LOADING;
}

void Texture::loadToGPU()
{
    assert(resourceHandle);
    auto& res = const_cast<ResourceTexture&>(resourceHandle.get());
    assert(isLoaded() && res.getStatus() == ResourceTexture::Status::LOADED);

    GraphicsSystem* gs = textureManager->getGraphicsSystem();
    llTexture =
    service::renderer::ref().createTexture
    (
        res.getTextureData(),
        res.getWidth(),
        res.getHeight()
    );
    llTexture.setFilterMode(filterMode);
    status = Status::READY;
}

void Texture::releaseResource()
{
    service::resource::ref().release(resourceHandle);
}

void Texture::release()
{
    if( isReady() )
    {
        // the texture is un VRAM
        GraphicsSystem* gs = textureManager->getGraphicsSystem();
        service::renderer::ref().destroyTexture(&llTexture);
    }
    else
    {
        // Even if the resource is still loading the ResourceManager will
        // take care of this
        releaseResource();
    }
}

void Texture::draw
(
    const Vec2f& pos,
    const Vec2f& scale,
    const LowLevelRenderer2D::SpriteVbo& sVbo
)const
{
    service::renderer::ref().draw(pos, scale, llTexture, sVbo);
}

void Texture::draw
(
    const Vec2f& pos,
    const LowLevelRenderer2D::SpriteVbo& sVbo
)const
{
    draw(pos, Vec2f(1, 1), sVbo);
}

unsigned Texture::getWidth()const
{
    return llTexture.getWidth();
}

unsigned Texture::getHeight()const
{
    return llTexture.getHeight();
}
