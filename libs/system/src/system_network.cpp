/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_network
*/

#include "system_network.hpp"
#include <cstddef>
#include <stdexcept>
#include <exception>

#if defined(LINUX)
    #include "string.h"
#elif defined(WIN32)
    #include <winbase.h>
    #include <winsock2.h>
#endif

#if defined(WIN32)
    #pragma comment(lib, "ws2_32.lib")
static WSADATA WData;

void System::Network::initNetwork(void)
{
    if (WSAStartup(MAKEWORD(2, 0), &WData) != 0) {
        throw std::runtime_error("Failed to initialize winsock2");
    }
}

void System::Network::stopNetwork(void)
{
    if (WSACleanup() != 0) {
        throw std::runtime_error("Failed to cleanup winsock2");
    }
}

#elif defined(LINUX)
void System::Network::initNetwork(void)
{
    return;
}

void System::Network::stopNetwork(void)
{
    return;
}
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

void System::Network::ASocket::closeSocket()
{
    this->_opened = false;
#if defined(LINUX)
    if (this->_sockfd != -1)
        if (close(this->_sockfd) == -1) {
            throw NetworkException("System::Network::ASocket::closeSocket: "
                                   "Failed to close socket");
        }
#elif defined(WIN32)
    if (closesocket(this->_sockfd) == SOCKET_ERROR) {
        throw NetworkException(
            "System::Network::ASocket::closeSocket: Failed to close socket");
    }
#endif
}

bool System::Network::ASocket::isOpen() const
{
    return (_opened);
}

uint64_t System::Network::ASocket::getUID() const
{
    return (_uid);
}

bool System::Network::ASocket::operator==(const System::Network::ASocket &target)
{
    return (target._uid == this->_uid);
}

bool System::Network::ASocket::operator!=(const System::Network::ASocket &target)
{
    return (target._uid != this->_uid);
}

osSocketType System::Network::ASocket::getHandle(void) const {
    return (this->_sockfd);
}
