
#ifndef VEC2F
#define VEC2F

class Vec2f
{
	
public:

	float x, y;
	
	Vec2f(){}
	Vec2f(float x, float y);
	
	Vec2f operator=(const Vec2f& vec);
	
	Vec2f operator+(const Vec2f& vec);
	Vec2f operator-(const Vec2f& vec);
	Vec2f operator-();
	
};

#endif
