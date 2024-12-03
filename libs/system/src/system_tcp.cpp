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

void TCPSocket::initSocket(
    uint16_t port, Mode mode, const std::string &address)
{
    int res;
    this->_mode = mode;
    this->_sockSettings.sin_family = AF_INET;
    this->_sockSettings.sin_port = htons(port);
    this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_sockfd == INVALID_SOCKET)
        throw NetworkException(
            "System::Network::TCPSocket::initSocket: Failed to create socket");

    if (mode == SERVE) {
        _sockSettings.sin_addr.s_addr = htonl(INADDR_ANY);
        res = bind(this->_sockfd,
            reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
            sizeof(this->_sockSettings));
        if (res == SOCKET_ERROR)
            throw NetworkException("System::Network::TCPSocket::initSocket: "
                                   "Failed to bind socket");
        res = listen(this->_sockfd, FD_SETSIZE);
        if (res == SOCKET_ERROR)
            throw NetworkException(
                "System::Network::TCPSocket::initSocket: Failed to listen");
        this->_opened = true;
    } else if (mode == CONNECT) {
        this->_sockSettings.sin_addr.s_addr = inet_addr(address.c_str());
        res = connect(this->_sockfd,
            reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
            sizeof(this->_sockSettings));
        if (res == SOCKET_ERROR)
            throw NetworkException(
                "System::Network::TCPSocket::initSocket: Failed to connect");
        this->_opened = true;
    }
}

TCPSocket::TCPSocket()
{
    tcpSockID++;
    this->_uid = tcpSockID;
    this->_opened = false;
}

TCPSocket::TCPSocket(uint16_t port, Mode mode, const std::string &address)
{
    tcpSockID++;
    this->_uid = tcpSockID;
    this->_opened = false;
    this->initSocket(port, mode, address);
}

ssize_t TCPSocket::sendData(const byteArray &byteSequence)
{
    ssize_t writtenBytes = 0;
    size_t len = byteSequence.size();
    uint8_t *buff = new uint8_t[len]();

    if (buff == NULL)
        throw std::runtime_error("System::Network::TCPSocket::sendData: "
                                 "Failed to allocate send buffer");
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
        throw NetworkException(
            "System::Network::TCPSocket::sendData: Failed to send");
    return (writtenBytes);
}

TCPSocket::TCPSocket(osSocketType sock_fd)
{
    tcpSockID++;
    this->_opened = true;
    this->_sockfd = sock_fd;
    this->_uid = tcpSockID;
    this->_mode = CONNECT;
    this->_sockSettings = {};
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
        delete[] buff;
        throw NetworkException(
            "System::Network::TCPSocket::receive: Failed to read");
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
    osSocketType res = 0;
    struct sockaddr clientAddr;
    socklen_t clientSockLen = sizeof(clientAddr);

    res = accept(src._sockfd, &clientAddr, &clientSockLen);
    if (res == INVALID_SOCKET || res == 0)
        throw NetworkException("System::Network::accept: Failed to accept "
                               "connection to TCP socket: ");
    return (Network::TCPSocket(res));
}

void System::Network::addSocketToSet(
    const std::vector<TCPSocket> &src, socketSetGeneric &dest)
{
    for (size_t i = 0; i != src.size(); i++) {
        auto *a = const_cast<TCPSocket *>(&(src.at(i)));
        dest.emplace_back(a);
    }
}

void System::Network::addSocketToSet(TCPSocket *src, socketSetGeneric &dest)
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
    System::Network::socketSetGeneric &set)
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
