#include "texture.hpp"
#include <cassert>
#include "graphics_system.hpp"

const std::string Texture::texturesPath = "img/";

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

bool core::resource::LoaderProxy<texture>::load_synchronously(texture* ptr)
{
    ptr->m_intermediate.setName(ptr->m_id);

    ptr->m_final.status = Texture::Status::START;
    ptr->m_final.llTexture = LowLevelRenderer2D::Texture();
    ptr->m_final.filterMode = Texture::FilterMode::NEAREST;

    return false;
}
bool core::resource::LoaderProxy<texture>::unload_synchronously(texture* ptr)
{
    service::renderer::ref().destroyTexture(&ptr->m_final.llTexture);

    return ptr->m_final.isReady();
}
bool core::resource::LoaderProxy<texture>::load_asynchronously(texture* ptr)
{
    ptr->m_final.status = Texture::Status::LOADING;

    ptr->m_intermediate.load();

    ptr->m_final.status = Texture::Status::LOADED;

    return true;
}
bool core::resource::LoaderProxy<texture>::unload_asynchronously(texture* ptr)
{
    ptr->m_intermediate.free();

    return true;
}
bool core::resource::LoaderProxy<texture>::synchronize_loaded(texture* ptr)
{
    ptr->m_final.llTexture = service::renderer::ref().createTexture
    (
        ptr->m_intermediate.getTextureData(),
        ptr->m_intermediate.getWidth(),
        ptr->m_intermediate.getHeight()
    );
    ptr->m_intermediate.free();

    ptr->m_final.llTexture.setFilterMode(ptr->m_final.filterMode);
    ptr->m_final.status = Texture::Status::READY;

    return true;
}


