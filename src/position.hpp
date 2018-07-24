#include "math/vec2f.hpp"
#include "math/vec2i.hpp"

#ifndef POSITION
#define POSITION

#include <cereal/cereal.hpp>
#include "util/xmlstr.hpp"

class Position
{
	
	Vec2i cell;
	Vec2f offset;
	
public:

	Position(){}
	Position(const Vec2i& cell, const Vec2f& offset);
	Position(const Position& pos);
	
	const Vec2i& getCell()const;
	const Vec2f& getOffset()const;
	
	Vec2i& getCell();
	Vec2f& getOffset();
	
	void setCell(const Vec2i& cell);
	void setOffset(const Vec2f& offset);
	
	Position operator=(const Position& pos);
	
	Position operator+(const Position& pos)const;
	Position operator-(const Position& pos)const;
	
	Position operator-()const;

    template<typename Archive>
    void load(Archive &archive);

    template<typename Archive>
    void save(Archive &archive) const;
};

#endif
