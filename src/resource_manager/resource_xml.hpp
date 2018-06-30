#pragma once

#include <string>
#include <rapidxml.hpp>

class ResourceXml
{
public:
    bool load(const char* path, const char* root_name);
    bool save(const char* path);
    bool free();

	rapidxml::xml_node<>* getNode();
	rapidxml::xml_document<>* getDocument();

private:
    std::string text;
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *node;
};
