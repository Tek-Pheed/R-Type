/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_network
*/

#include "system_network.hpp"
#include <cstddef>
#include <exception>
#include <stdexcept>

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

bool System::Network::ASocket::operator==(
    const System::Network::ASocket &target)
{
    return (target._uid == this->_uid);
}

bool System::Network::ASocket::operator!=(
    const System::Network::ASocket &target)
{
    return (target._uid != this->_uid);
}

osSocketType System::Network::ASocket::getHandle(void) const
{
    return (this->_sockfd);
}

System::Network::ISocket::Type System::Network::ASocket::getType() const
{
    return (_type);
}

System::Network::byteArray System::Network::encodeString(
    const std::string &str)
{
    byteArray br;
    size_t len = str.length();

    br.reserve(len);
    for (size_t i = 0; i < len; i++) {
        br.emplace_back(str[i]);
    }
    return (br);
}

void System::Network::select(socketSetGeneric *readfds,
    socketSetGeneric *writefds, socketSetGeneric *exceptfds,
    timeoutStruct timeout)
{
    fd_set read_set;
    fd_set write_set;
    fd_set except_set;
    fd_set *read_set_p = NULL;
    fd_set *write_set_p = NULL;
    fd_set *except_set_p = NULL;
    s_timeval t = {0, 0};
    s_timeval *tm = NULL;

    if (timeout.has_value()) {
        t.tv_sec = timeout->tv_sec;
        t.tv_usec = timeout->tv_usec;
        tm = &t;
    }
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&except_set);
    if (readfds != nullptr && readfds->size() > 0) {
        read_set_p = &read_set;
        for (const auto *i : *readfds) {
            if (i != nullptr)
                FD_SET(i->getHandle(), read_set_p);
        }
    }
    if (writefds != nullptr && writefds->size() > 0) {
        write_set_p = &write_set;
        for (const auto *i : *writefds) {
            if (i != nullptr)
                FD_SET(i->getHandle(), write_set_p);
        }
    }
    if (exceptfds != nullptr && exceptfds->size() > 0) {
        except_set_p = &except_set;
        for (const auto *i : *exceptfds) {
            if (i != nullptr)
                FD_SET(i->getHandle(), except_set_p);
        }
    }
    if (select(FD_SETSIZE, read_set_p, write_set_p, except_set_p, tm)
        == SOCKET_ERROR)
        throw NetworkException("Network::select failed");
    if (readfds != nullptr) {
        socketSetGeneric readCopy = *readfds;
        readfds->clear();
        for (size_t i = 0; i < readCopy.size(); i++) {
            if (readCopy[i] != nullptr
                && FD_ISSET(readCopy[i]->getHandle(), read_set_p)) {
                readfds->emplace_back(readCopy[i]);
            }
        }
    }
    if (writefds != nullptr) {
        socketSetGeneric writeCopy = *writefds;
        writefds->clear();
        for (size_t i = 0; i < writeCopy.size(); i++) {
            if (writeCopy[i] != nullptr
                && FD_ISSET(writeCopy[i]->getHandle(), write_set_p)) {
                writefds->emplace_back(writeCopy[i]);
            }
        }
    }
    if (exceptfds != nullptr) {
        socketSetGeneric exceptCopy = *exceptfds;
        exceptfds->clear();
        for (size_t i = 0; i < exceptCopy.size(); i++) {
            if (exceptCopy[i] != nullptr
                && FD_ISSET(exceptCopy[i]->getHandle(), except_set_p)) {
                exceptfds->emplace_back(exceptCopy[i]);
            }
        }
    }
}