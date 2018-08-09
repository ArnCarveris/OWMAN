#pragma once
#include <sstream>

namespace core::serialization
{
    template<class Input, class Output>
    class Archive
    {
    public:

        template <typename... Args>
        Input& input(Args &&... args)
        {
            if (!m_input)
            {
                m_input = std::make_unique<Input>(m_buffer, std::forward<Args>(args)...);
            }

            return *m_input;
        }
        template <typename... Args>
        Output& output(Args &&... args)
        {
            if (!m_output)
            {
                clear();

                m_output = std::make_unique<Output>(m_buffer, std::forward<Args>(args)...);
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
    protected:
        std::stringstream       m_buffer;
        std::unique_ptr<Input>  m_input;
        std::unique_ptr<Output> m_output;
    };
}
