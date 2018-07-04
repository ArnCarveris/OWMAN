#ifndef GRAPHICS_SYSTEM
#define GRAPHICS_SYSTEM

#include "low_level_renderer_2d.hpp"
#include <string>
#include <set>
#include <vector>
#include "camera.hpp"
#include <rapidxml.hpp>
#include "../entity.hpp"

class Engine;
class Sprite;
class SpriteStatus;

class GraphicsSystem
{

	Engine* myEngine;

	Camera camera;

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

    void assignComponent(EntityRegistry& registry, Entity entity, rapidxml::xml_node<>* node);

    void destroyComponent(EntityRegistry& registry, Entity entity);

    rapidxml::xml_node<>* createXmlNode(EntityRegistry& registry, Entity entity, rapidxml::xml_document<>* doc);

	Camera* getCamera();

	void end();

};

#endif
