
#include "util/xml.hpp"
#include "util/xmlstr.hpp"

template<typename Archive>
void Position::load(Archive &archive) 
{
    cereal::make_optional_nvp(archive, xmlstr::cell_x, cell.x);
    cereal::make_optional_nvp(archive, xmlstr::cell_y, cell.y);

    archive(cereal::make_nvp(xmlstr::x, offset.x));
    archive(cereal::make_nvp(xmlstr::y, offset.y));
}
template<typename Archive>
void Position::save(Archive &archive) const 
{
    if (cell.x != 0 || cell.y != 0)
    {
        archive(cereal::make_nvp(xmlstr::cell_x, cell.x));
        archive(cereal::make_nvp(xmlstr::cell_y, cell.y));
    }
    
    archive(cereal::make_nvp(xmlstr::x, offset.x));
    archive(cereal::make_nvp(xmlstr::y, offset.y));
}
