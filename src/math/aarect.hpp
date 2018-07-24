#ifndef AARECT_HPP_INCLUDED
#define AARECT_HPP_INCLUDED

#include <cereal/cereal.hpp>

/* Axially aligned rectangle */
class AARect{

public:

    float x = 0.0f, y = 0.0f;
    float w = 0.0f, h = 0.0f;

    AARect() = default;
    AARect(AARect&&) = default;
    AARect(const AARect&) = default;

    AARect(float x, float y, float w, float h);

    AARect& operator=(AARect&&) = default;
    AARect& operator=(const AARect&) = default;
};

template<typename Archive>
void serialize(Archive &archive, AARect &ref) {
    archive(cereal::make_nvp("x", ref.x));
    archive(cereal::make_nvp("y", ref.y));
    archive(cereal::make_nvp("w", ref.w));
    archive(cereal::make_nvp("h", ref.h));
}


#endif // AARECT_HPP_INCLUDED
