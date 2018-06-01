#pragma once

#include <string>
#include <exception>

namespace mbs{
    class MbsException : public std::exception{
    public:
        MbsException(std::string message) : m_message(std::move(message)) {}
        const char *what() const noexcept override { return m_message.c_str(); }
    private:
        std::string m_message;

    };
}


