#include <vector>
#include "math/vec2i.hpp"
#include "math/vec2f.hpp"

#ifndef ENTITY
class Entity;
#endif

#ifndef MAIN_CHARACTER
class MainCharacter;
#endif

#ifndef ENTITY_FACTORY
class EntityFactory;
#endif

#ifndef I_WORLD_STREAMER
#define I_WORLD_STREAMER

#include <entt/locator/locator.hpp>

class IWorldStreamer
{
public:
	/**
	 * \brief Inits the world streamer.
	 * \param cell the cell where the main character is located.
	 * \param offset of the character wrt the cell bot left corner.
	 * Both params could be ignored by the implementation.
	 **/
	virtual void init(const Vec2i& cell, const Vec2f& offset) = 0;

	/**
	 * \brief Updates the world streamer.
	 * \param offset of the character wrt the cell bot left corner.
	 * The param could be ignored by the implementation.
	 **/
	virtual void update(const Vec2f& position, MainCharacter* mainCharacter) = 0;

	/**
	 * \brief Returns a vector of pointers to all the active entities
	 *
	 **/
	virtual std::vector<Entity*> getEntities()const = 0;

	virtual float getCellSize()const;

	virtual void end() = 0;

	virtual ~IWorldStreamer(){}

};


namespace service
{
    using world_streamer = entt::ServiceLocator<IWorldStreamer>;
}

#endif
