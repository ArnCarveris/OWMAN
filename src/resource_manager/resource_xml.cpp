#include "resource_xml.hpp"
#include <rapidxml_print.hpp>
#include <iostream>
#include <fstream>


bool ResourceXml::load(const char* path, const char* root_name)
{
    std::fstream fs;
	fs.open(path, std::fstream::in);
	fs.seekg(0, fs.end);
	int length = fs.tellg();
	fs.seekg(0, fs.beg);
    text.resize(length + 1);

	fs.read(&text[0], length);
	text[length] = '\0';
	fs.close();

   // cout << name << endl;
    /*cout << text << endl;
    cout << fs.gcount() << endl;
    */
	doc.parse<0>(&text[0]);

    node = doc.first_node(root_name);

    return node != nullptr;
}
bool ResourceXml::save(const char* path)
{
    std::fstream fs;
    fs.open(path, std::fstream::binary | std::fstream::out);

    fs << doc << std::endl;

    return true;
}
bool ResourceXml::free()
{
    text.clear();

    return true;
}

rapidxml::xml_node<>* ResourceXml::getNode()
{
    return node;
}

rapidxml::xml_document<>* ResourceXml::getDocument()
{
    return &doc;
}