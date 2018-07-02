#ifndef SPRITE
#define SPRITE

#include "low_level_renderer_2d.hpp"
#include "../math/vec2f.hpp"
#include "../resource_manager/resource_manager.hpp"
#include "../resource_manager/resource_xml.hpp"

#include <map>
#include <vector>
#include "animation.hpp"
#include "texture.hpp"

class GraphicsSystem;
class SpriteStatus;

/** \brief Represents a static sprite
 * This sprite has no animations
 */
class Sprite
{
public:
    // constants
    static const std::string spritesPath;
public:
    enum class Status
    {
        START,
        LOADING_XML,
        LOADED_XML,
        LOADING_TEXTURES,
        LOADED_TEXTURES,
        EVERYTHING_LOADED
    };

	friend class SpriteStatus;
    using Resource = core::resource::Data<ResourceXml, Sprite, Status>;

	Sprite() = default;

	unsigned getAnimIndex(const std::string& animName)const;

	~Sprite() = default;
private:
    std::map<std::string, unsigned> textureNameToIndex;
    std::vector<core::resource::Handle<Texture::Resource>> textures;

    std::map<std::string, unsigned> animNameToIndex;
    std::map<std::string, unsigned> idToIndex;
    std::vector<Animation> animations;
private:
    friend struct core::resource::LoaderProxy<Resource>;
};

#endif
