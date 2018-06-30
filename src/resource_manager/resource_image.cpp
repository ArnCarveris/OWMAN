#include "resource_image.hpp"
#include <iostream>
#include <stb_image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

using namespace std;


const unsigned char* ResourceImage::getData()const
{
    return data;
}

int ResourceImage::getWidth()const
{
    return width;
}

int ResourceImage::getHeight()const
{
    return height;
}


bool ResourceImage::load(const char* path)
{
	data = stbi_load
	(
        path,	// path
		&width,				// pointer where width will be saved
		&height,				// pointer where height will be saved
		0,					// pointer where num of chanel will be saved
        STBI_rgb_alpha 		// desired chanels
	);

    return data != nullptr;
}

bool ResourceImage::free()
{
    if (data == nullptr)
    {
        return false;
    }

	stbi_image_free(data);

    data = nullptr;
    width = 0;
    height = 0;

    return true;
}