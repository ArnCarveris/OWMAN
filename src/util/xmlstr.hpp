/**
* Here you will find shared strings for xml nodes
**/

#ifndef XMLSTR
#define XMLSTR

#include <array>

namespace xmlstr
{

    extern const char* entity;
    extern const char* space;
    extern const char* x;
    extern const char* y;
    extern const char* cell_x;
    extern const char* cell_y;
    extern const char* width;
    extern const char* height;
    extern const char* radius;
    extern const char* position;
    extern const char* graphics;
    extern const char* physics;
    extern const char* shape;
    extern const char* box;
    extern const char* circle;
    extern const char* priority;
    extern const char* sprite;
    extern const char* mass;
    extern const char* kinematic;

    template<typename Archive>
    bool has(Archive& archive, const char* name)
    {
        const char* current = archive.getNodeName();

        return current != nullptr && !strcmp(current, name);
    }

    const std::array<const char*, 1> tags = {
        "main_character"
    };

    const std::array<const char*, 3> components = {
        position,
        graphics,
        physics
    };
}

#endif // XMLSTR
