#pragma once

#include <iostream>
#include <fstream>

#include "../util/xml.hpp"
#include "../util/archive.hpp"

template<class Input, class Output>
class ResourceArchive : public core::serialization::Archive<Input, Output>
{
public:
    bool load(const char* path)
    {
        std::ifstream file(path);

        if (file)
        {
            m_buffer << file.rdbuf();

            file.close();

            return true;
        }
        else
        {
            return false;
        }
    }
    bool save(const char* path)
    {
        std::fstream fs;
        fs.open(path, std::fstream::binary | std::fstream::out);

        fs << m_buffer.rdbuf() << std::endl;

        return true;
    }
};


using ResourceXMLRootArchive = ResourceArchive<cereal::XMLRootInputArchive, cereal::XMLRootOutputArchive>;
