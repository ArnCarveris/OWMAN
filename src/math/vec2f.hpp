
#ifndef VEC2F
#define VEC2F

class Vec2i;

/**
 * Represents a vector of two float components
 */
class Vec2f
{
public:
    template<typename...Tags>
    struct RepositionEvent
    {
        Vec2f from;
        Vec2f to;
    };
public:

	float x, y;

	Vec2f(){}
	Vec2f(float x, float y);
	Vec2f(const Vec2f& v);
	Vec2f(const Vec2i& v);

	Vec2f operator=(const Vec2f& vec);

    Vec2f operator+=(const Vec2f& vec);
    Vec2f operator-=(const Vec2f& vec);

	Vec2f operator+(const Vec2f& vec)const;
	Vec2f operator-(const Vec2f& vec)const;
	Vec2f operator-()const;

};

template<typename Archive>
void serialize(Archive &archive, Vec2f &ref) {
    archive(cereal::make_nvp("x", ref.x));
    archive(cereal::make_nvp("y", ref.y));
}

#endif
