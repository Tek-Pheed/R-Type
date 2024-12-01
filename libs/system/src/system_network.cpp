/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_network
*/

#include "system_network.hpp"
#include <cstddef>
#include "system.hpp"

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

void System::Network::addSocketToSet(
    const std::vector<System::Network::TCPSocket> &src, socketSetTCP &dest)
{
    for (size_t i = 0; i != src.size(); i++) {
        TCPSocket *a = const_cast<TCPSocket *>(&(src.at(i)));
        dest.emplace_back(a);
    }
}

void System::Network::addSocketToSet(TCPSocket *src, socketSetTCP &dest)
{
    dest.emplace_back(src);
}

bool System::Network::removeSocketInVect(const System::Network::TCPSocket &toRemove,
    std::vector<System::Network::TCPSocket> &vect)
{
    const uint64_t uid = toRemove.getUID();

    for (size_t i = 0; i != vect.size(); i++) {
        if (vect[i].getUID() == uid) {
            vect.erase(vect.begin() + static_cast<long>(i));
            return (true);
        }
    }
    return (false);
}

bool System::Network::removeSocketInSet(const System::Network::TCPSocket &toRemove,
    System::Network::socketSetTCP &set)
{
    const uint64_t uid = toRemove.getUID();

    for (size_t i = 0; i != set.size(); i++) {
        if (set[i]->getUID() == uid) {
            set.erase(set.begin() + static_cast<long>(i));
            return (true);
        }
    }
    return (false);
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
