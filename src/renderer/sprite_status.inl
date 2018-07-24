
#include <string>
#include "../util/xmlstr.hpp"
#include <cereal/cereal.hpp>

template<typename Archive>
void SpriteStatus::serialize(Archive &archive) {

    archive(cereal::make_nvp(xmlstr::sprite, sprite));
    archive(cereal::make_nvp(xmlstr::width, scale.x));
    archive(cereal::make_nvp(xmlstr::height, scale.y));

    try {
        archive(cereal::make_nvp(xmlstr::priority, priority));
    }
    catch (const std::exception e) { (void)e; }
}