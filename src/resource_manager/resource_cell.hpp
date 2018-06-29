#ifndef RESOURCE_CELL
#define RESOURCE_CELL

#include "resource.hpp"
#include <rapidxml.hpp>

/** \brief Cell resource
 * This class is a modifiable resource.
 * When there are no references to this resource
 * the changes will be stored.
 */
class ResourceCell : public Resource
{
    char* text;
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *node;

public:
	ResourceCell(const std::string& name = "");

    void load();
    void free();

	rapidxml::xml_node<>* getNode();
	rapidxml::xml_document<>* getDocument();

	~ResourceCell(){}

};

#endif
