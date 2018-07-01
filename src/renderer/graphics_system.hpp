#ifndef GRAPHICS_SYSTEM
#define GRAPHICS_SYSTEM

#include "low_level_renderer_2d.hpp"
#include <string>
#include <set>
#include <vector>
#include "camera.hpp"

class Engine;
class GraphicsComponent;
class Sprite;
class SpriteStatus;

class GraphicsSystem
{

	Engine* myEngine;

	Camera camera;

	std::set<GraphicsComponent*> components;

public:

	GraphicsSystem
	(
		std::string windowTitle,
		unsigned int width, unsigned int height,
		bool fullScreen
	);

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

	SpriteStatus* instanceSprite(std::string fileName, const Vec2f& scale = Vec2f(1, 1));

	void destroyGraphicsComponent(GraphicsComponent* graphicsComponent);

	Camera* getCamera();

	void end();

};

#endif
