#ifndef AARECT_HPP_INCLUDED
#define AARECT_HPP_INCLUDED

#include <cereal/cereal.hpp>

/* Axially aligned rectangle */
class AARect{

public:

    float x, y;
    float w, h;

    AARect(){}
    AARect(float x, float y, float w, float h);
    AARect(const AARect& rect);

    AARect operator=(const AARect& rect);

};

template<typename Archive>
void serialize(Archive &archive, AARect &ref) {
    archive(cereal::make_nvp("x", ref.x));
    archive(cereal::make_nvp("y", ref.y));
    archive(cereal::make_nvp("w", ref.w));
    archive(cereal::make_nvp("h", ref.h));
}


#endif // AARECT_HPP_INCLUDED
