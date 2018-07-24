
#ifndef VEC2I
#define VEC2I

class Vec2f;

/**
 * Represents a vector of two integer components
 *
 */
class Vec2i
{
public:
    template<typename...Tags>
    struct RepositionEvent
    {
        Vec2i from;
        Vec2i to;
    };

public:

	int x = 0, y = 0;

    Vec2i() = default;
    Vec2i(Vec2i&&) = default;
    Vec2i(const Vec2i&) = default;
	Vec2i(int x, int y);
	Vec2i(const Vec2f& v);

    Vec2i& operator=(Vec2i&&) = default;
    Vec2i& operator=(const Vec2i&) = default;

	Vec2i& operator+=(const Vec2i& vec);
	Vec2i& operator-=(const Vec2i& vec);

	Vec2i operator+(const Vec2i& vec)const;
	Vec2i operator-(const Vec2i& vec)const;
	Vec2i operator-()const;

	bool operator<(const Vec2i& v)const;
	bool operator==(const Vec2i& v)const;
	bool operator!=(const Vec2i& v)const;

};

template<typename Archive>
void serialize(Archive &archive, Vec2i &ref) {
    archive(cereal::make_nvp("x", ref.x));
    archive(cereal::make_nvp("y", ref.y));
}

#endif
