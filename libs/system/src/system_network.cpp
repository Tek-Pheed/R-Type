/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_network
*/

#include "system_network.hpp"
#include <cstddef>

#if defined(LINUX)
    #include "string.h"
#elif defined(WIN32)
    #include <winbase.h>
    #include <winsock2.h>
#endif

System::Network::NetworkException::NetworkException(const std::string &str)
    : _content(str)
{
#if defined(LINUX)
    _content += " : ";
    _content += strerror(errno);

#elif defined(WIN32)
    char msgbuf[256]{'\0'};
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        msgbuf, sizeof(msgbuf), NULL);
    _content += " : ";
    if (!*msgbuf) {
        _content += "Unknown error";
    } else {
        _content += (std::string(msgbuf));
    }
#endif
}

const char *System::Network::NetworkException::what() const noexcept
{
    return (this->_content.c_str());
}

std::string System::Network::decodeString(
    const System::Network::byteArray &bytes)
{
    std::string s;

    s.reserve(bytes.size());
    for (const auto &b : bytes) {
        s += static_cast<char>(b);
    }
    return (s);
}
