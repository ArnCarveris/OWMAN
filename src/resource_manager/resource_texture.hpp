#ifndef RESOURCE_TEXTURE
#define RESOURCE_TEXTURE

#include "resource.hpp"
#include "../renderer/low_level_renderer_2d.hpp"

class ResourceManager;


/** \brief Represents a resource texture
 * Since OpenGL does not allow to make any call
 * from any other thread other than the main thread,
 * the resource manager can not load anything to video
 * memory. So for this resource, to be LOADED means to
 * be loaded in main memory and to be READY means to be
 * loaded in video memory
 */
class ResourceTexture : public Resource
{
	unsigned char* imageData;
	int width, height;


public:

	ResourceTexture(const std::string& name = "");

    void load();
    void free();

	const unsigned char* getTextureData()const;
	int getWidth()const;
	int getHeight()const;

	~ResourceTexture(){}

};

#endif
