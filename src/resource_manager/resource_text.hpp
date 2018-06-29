#ifndef RESOURCE_TEXT
#define RESOURCE_TEXT

#include "resource.hpp"

class ResourceManager;

class ResourceText : public Resource
{
	std::string text;

public:

    ResourceText(const std::string& name = "");

    void load();
    void free();


	const std::string& getText()const;


	~ResourceText(){}

};

#endif
