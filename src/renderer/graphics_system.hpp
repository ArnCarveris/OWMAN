#ifndef GRAPHICS_SYSTEM
#define GRAPHICS_SYSTEM

#include "low_level_renderer_2d.hpp"
#include <string>
#include <set>
#include <vector>
#include "camera.hpp"
#include "../entity.hpp"

class GraphicsSystem
{
	Camera camera;

public:

	GraphicsSystem();

    /** \brief update animations
     */
	void update(unsigned int delta);

    /** \brief set fullscreen mode
     */
    void setFullScreen(bool b);

	void draw();

	/** \brief Swap buffers
	 * Once you ave drawn all the components \
	 * call this function to swap buffers
	 */
	void swap();

    void destroyComponent(EntityRegistry& registry, Entity entity);

	Camera* getCamera();

	void end();

    template<typename Archive>
    void load(Archive& ar);

    template<typename Archive>
    void save(Archive&) const;
};

#endif
