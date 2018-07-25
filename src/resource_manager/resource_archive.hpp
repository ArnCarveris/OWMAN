#pragma once

#include "../util/xml.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

template<class Input, class Output>
class ResourceArchive
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

    Input& input(const char* root)
    {
        if (!m_input)
        {
            m_input = std::make_unique<Input>(m_buffer, root);
        }

        return *m_input;
    }
    Output& output(const char* root)
    {
        if (!m_output)
        {
            clear();

            m_output = std::make_unique<Output>(m_buffer, root);
        }

        return *m_output;
    }

    void finalize_input()
    {
        m_input.reset();
    }

    void finalize_output()
    {
        m_output.reset();
    }

    void clear()
    {
        m_buffer.str(std::string());
        m_buffer.clear();
    }

    bool empty() const
    {
        return m_buffer.str().empty();
    }
private:
    std::stringstream       m_buffer;
    std::unique_ptr<Input>  m_input;
    std::unique_ptr<Output> m_output;
};