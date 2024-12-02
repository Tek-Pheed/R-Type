/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_tcp
*/

#include "system_tcp.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include "system_network.hpp"

using namespace System;
using namespace Network;

static uint64_t tcpSockID = 0;

TCPSocket::TCPSocket(uint16_t port, Mode mode, const std::string &address)
{
    tcpSockID++;
    int res;
    this->_mode = mode;
    this->_opened = false;
    this->_sockSettings.sin_family = AF_INET;
    this->_sockSettings.sin_port = htons(port);
    this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    this->_uid = tcpSockID;
    if (this->_sockfd == INVALID_SOCKET)
        throw NetworkException(
            "System::Network::TCPSocket: Failed to create socket");

    if (mode == SERVE) {
        _sockSettings.sin_addr.s_addr = htonl(INADDR_ANY);
        res = bind(this->_sockfd,
            reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
            sizeof(this->_sockSettings));
        if (res == SOCKET_ERROR)
            throw NetworkException(
                "System::Network::TCPSocket: Failed to bind socket");
        res = listen(this->_sockfd, FD_SETSIZE);
        if (res == SOCKET_ERROR)
            throw NetworkException(
                "System::Network::TCPSocket: Failed to listen");
        this->_opened = true;
    } else if (mode == CONNECT) {
        this->_sockSettings.sin_addr.s_addr = inet_addr(address.c_str());
        res = connect(this->_sockfd,
            reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
            sizeof(this->_sockSettings));
        if (res == SOCKET_ERROR)
            throw NetworkException(
                "System::Network::TCPSocket: Failed to connect");
        this->_opened = true;
    }
}

ssize_t TCPSocket::sendData(const byteArray &byteSequence)
{
    ssize_t writtenBytes = 0;
    size_t len = byteSequence.size();
    uint8_t *buff = new uint8_t[len]();

    if (buff == NULL)
        throw std::runtime_error(
            "System::Network::TCPSocket::sendData: Failed to allocate send buffer");
    for (size_t i = 0; i < len; i++) {
        buff[i] = byteSequence[i];
    }
#if defined(LINUX)
    writtenBytes = write(this->_sockfd, reinterpret_cast<void *>(buff), len);
#elif defined(WIN32)
    writtenBytes =
        send(this->_sockfd, reinterpret_cast<const char *>(buff), len, 0);
#endif
    this->_opened = (writtenBytes > 0);
    delete[] buff;
    if (writtenBytes == SOCKET_ERROR)
        throw NetworkException("System::Network::TCPSocket::sendData: Failed to send");
    return (writtenBytes);
}

TCPSocket::TCPSocket(osSocketType sock_fd)
{
    tcpSockID++;
    this->_opened = true;
    this->_sockfd = sock_fd;
    this->_uid = tcpSockID;
    this->_mode = CONNECT;
}

TCPSocket::~TCPSocket()
{
}

byteArray TCPSocket::receive(void)
{
    ssize_t ret = 0;
    byteArray vect;
    uint8_t dummy = 0;

#if defined(WIN32)
    u_long len = 0;
    ioctlsocket(this->_sockfd, FIONREAD, &len);
#elif defined(LINUX)
    size_t len = 0;
    ioctl(this->_sockfd, FIONREAD, &len);
#endif

    if (len == 0) {
#if defined(LINUX)
        ret = read(this->_sockfd, &dummy, len);
#elif defined(WIN32)
        ret = recv(this->_sockfd, reinterpret_cast<char *>(&dummy), len, 0);
        if (this->_opened && ret == SOCKET_ERROR) {
            this->_opened = (ret > 0);
            return (vect);
        }
#endif
        this->_opened = (ret > 0);
        if (ret == SOCKET_ERROR)
            throw NetworkException(
                "System::Network::TCPSocket::receive: Failed to read");
        return (vect);
    }
    uint8_t *buff = new uint8_t[len]();
#if defined(LINUX)
    if (buff == nullptr)
        throw std::runtime_error("System::Network::TCPSocket::receive: Unable "
                                 "to allocate receive buffer");
    ret = read(this->_sockfd, buff, len);

#elif defined(WIN32)
    ret = recv(this->_sockfd, reinterpret_cast<char *>(buff), len, 0);
#endif
    if (ret == SOCKET_ERROR) {
        delete [] buff;
        throw NetworkException("System::Network::TCPSocket::receive: Failed to read");
    }
    this->_opened = (ret > 0);
    vect.reserve(len);
    for (size_t i = 0; i < len; i++) {
        uint8_t val = buff[i];
        vect.emplace_back(val);
    }
    delete[] buff;
    return (vect);
}

TCPSocket Network::accept(const TCPSocket &src)
{
    int res = 0;
    struct sockaddr clientAddr;
    socklen_t clientSockLen = sizeof(clientAddr);

    res = accept(src._sockfd, reinterpret_cast<struct sockaddr *>(&clientAddr),
        &clientSockLen);
    if (res == INVALID_SOCKET || res == 0)
        throw NetworkException("System::Network::accept: Failed to accept "
                               "connection to TCP socket: ");
    return (Network::TCPSocket(res));
}

void Network::select(socketSetTCP *readfds, socketSetTCP *writefds,
    socketSetTCP *exceptfds, timeoutStruct timeout)
{
    fd_set read_set;
    fd_set write_set;
    fd_set except_set;
    fd_set *read_set_p = NULL;
    fd_set *write_set_p = NULL;
    fd_set *except_set_p = NULL;
    s_timeval *tm = NULL;

    if (timeout.has_value()) {
        tm = &timeout.value();
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
    if (select(FD_SETSIZE, read_set_p, write_set_p, except_set_p, tm) == SOCKET_ERROR)
        throw NetworkException("Network::select failed");
    if (readfds != nullptr && readfds->size() > 0) {
        for (size_t i = 0; i < readfds->size(); i++) {
            if ((*readfds)[i] != nullptr
                && !FD_ISSET((*readfds)[i]->getHandle(), read_set_p)) {
                readfds->erase((readfds->begin()) + (long) i);
            }
        }
    }
    if (writefds != nullptr && writefds->size() > 0) {
        for (size_t i = 0; i < writefds->size(); i++) {
            if ((*writefds)[i] != nullptr
                && !FD_ISSET((*writefds)[i]->getHandle(), write_set_p)) {
                writefds->erase((writefds->begin()) + (long) i);
            }
        }
    }
    if (exceptfds != nullptr && exceptfds->size() > 0) {
        for (size_t i = 0; i < exceptfds->size(); i++) {
            if ((*exceptfds)[i] != nullptr
                && !FD_ISSET((*exceptfds)[i]->getHandle(), except_set_p)) {
                exceptfds->erase((exceptfds->begin()) + (long) i);
            }
        }
    }
}

void System::Network::addSocketToSet(
    const std::vector<TCPSocket> &src, socketSetTCP &dest)
{
    for (size_t i = 0; i != src.size(); i++) {
        auto *a = const_cast<TCPSocket *>(&(src.at(i)));
        dest.emplace_back(a);
    }
}

void System::Network::addSocketToSet(TCPSocket *src, socketSetTCP &dest)
{
    dest.emplace_back(src);
}

bool System::Network::removeSocketInVect(
    const System::Network::TCPSocket &toRemove,
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

bool System::Network::removeSocketInSet(
    const System::Network::TCPSocket &toRemove,
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
