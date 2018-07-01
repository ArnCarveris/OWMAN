#include "texture.hpp"
#include <cassert>
#include "graphics_system.hpp"

const std::string Texture::texturesPath = "img/";

void Texture::setFilterMode(FilterMode filterMode)
{
    this->filterMode = filterMode;
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

bool core::resource::LoaderProxy<Texture::Resource>::load_synchronously(Texture::Resource* ptr)
{
    ptr->m_status = Texture::Status::START;
    ptr->m_final.llTexture = LowLevelRenderer2D::Texture();
    ptr->m_final.filterMode = Texture::FilterMode::NEAREST;

    return false;
}
bool core::resource::LoaderProxy<Texture::Resource>::unload_synchronously(Texture::Resource* ptr)
{
    service::renderer::ref().destroyTexture(&ptr->m_final.llTexture);

    return ptr->m_status == Texture::Status::READY;
}
bool core::resource::LoaderProxy<Texture::Resource>::load_asynchronously(Texture::Resource* ptr)
{
    ptr->m_status = Texture::Status::LOADING;

    auto path = Texture::texturesPath + ptr->m_id;

    ptr->m_intermediate.load(path.c_str());

    ptr->m_status = Texture::Status::LOADED;

    return true;
}
bool core::resource::LoaderProxy<Texture::Resource>::unload_asynchronously(Texture::Resource* ptr)
{
    ptr->m_intermediate.free();

    return true;
}
bool core::resource::LoaderProxy<Texture::Resource>::synchronize_loaded(Texture::Resource* ptr)
{
    ptr->m_final.llTexture = service::renderer::ref().createTexture
    (
        ptr->m_intermediate.getData(),
        ptr->m_intermediate.getWidth(),
        ptr->m_intermediate.getHeight()
    );
    ptr->m_intermediate.free();

    ptr->m_final.llTexture.setFilterMode(ptr->m_final.filterMode);
    ptr->m_status = Texture::Status::READY;

    return true;
}


