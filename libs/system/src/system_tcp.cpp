/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_tcp
*/

#include "system_tcp.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "system_network.hpp"

using namespace System;
using namespace Network;

static u_int64_t sockID = 0;

#if defined(LINUX)
TCPSocket::TCPSocket(uint16_t port, TCPMode mode, const std::string &address)
{
    sockID++;
    int res;
    this->_sockSettings.sin_family = AF_INET;
    this->_sockSettings.sin_port = htons(port);
    this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    this->_uid = sockID;
    if (this->_sockfd == -1)
        throw NetworkException(
            "System::Network::TCPSocket: Failed to create socket");

    if (mode == TCPMode::SERVE) {
        _sockSettings.sin_addr.s_addr = htonl(INADDR_ANY);
        res = bind(this->_sockfd,
            reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
            sizeof(this->_sockSettings));
        if (res == -1)
            throw NetworkException(
                "System::Network::TCPSocket: Failed to bind socket");
        res = listen(this->_sockfd, FD_SETSIZE);
        if (res == -1)
            throw NetworkException(
                "System::Network::TCPSocket: Failed to listen");
        this->_opened = true;
    } else if (mode == TCPMode::CONNECT) {
        this->_sockSettings.sin_addr.s_addr = inet_addr(address.c_str());
        res = connect(this->_sockfd,
            reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
            sizeof(this->_sockSettings));
        if (res == -1)
            throw NetworkException(
                "System::Network::TCPSocket: Failed to connect");
        this->_opened = true;
    }
}

TCPSocket::TCPSocket(osSocketType sock_fd)
{
    sockID++;
    this->_opened = true;
    this->_sockfd = sock_fd;
    this->_uid = sockID;
}

TCPSocket::~TCPSocket()
{
}

uint64_t TCPSocket::getUID() const
{
    return (_uid);
}

bool TCPSocket::operator==(const TCPSocket &target)
{
    return (target._sockfd == this->_sockfd);
}

bool TCPSocket::operator!=(const TCPSocket &target)
{
    return (target._sockfd != this->_sockfd);
}

void TCPSocket::closeSocket(void)
{
    this->_opened = false;
    #if defined(LINUX)
    if (this->_sockfd != -1)
        close(this->_sockfd);
    #elif defined(WIN32)
    return;
    #endif
}

bool TCPSocket::isOpen(void)
{
    return this->_opened;
}

ssize_t TCPSocket::send(const byteArray &byteSequence)
{
    ssize_t writtenBytes = 0;
    size_t len = byteSequence.size();
    uint8_t *buff = reinterpret_cast<uint8_t *>(
        calloc(byteSequence.size(), sizeof(uint8_t)));

    for (size_t i = 0; i < len; i++) {
        buff[i] = byteSequence[i];
    }
    writtenBytes = write(this->_sockfd, reinterpret_cast<void *>(buff), len);
    this->_opened = (writtenBytes > 0);
    if (writtenBytes == -1)
        throw NetworkException("System::Network::TCPSocket: Failed to send");
    free(buff);
    return (writtenBytes);
}

byteArray TCPSocket::receive(void)
{
    ssize_t ret = 0;
    byteArray vect;
    size_t len = 0;
    uint8_t dummy = 0;

    ioctl(this->_sockfd, FIONREAD, &len);
    if (len == 0) {
        ret = read(this->_sockfd, &dummy, len);
        this->_opened = (ret > 0);
        if (ret == -1)
            throw NetworkException(
                "System::Network::TCPSocket: Failed to read");
        return (vect);
    }
    uint8_t *buff = reinterpret_cast<uint8_t *>(calloc(len, sizeof(uint8_t)));
    if (buff == nullptr)
        throw std::runtime_error("System::Network::TCPSocket::receive: Unable "
                                 "to allocate receive buffer");
    ret = read(this->_sockfd, buff, len);
    if (ret == -1)
        throw NetworkException("System::Network::TCPSocket: Failed to read");
    this->_opened = (ret > 0);
    vect.reserve(len);
    for (size_t i = 0; i < len; i++)
        vect.emplace_back(buff[i]);
    free(buff);
    return (vect);
}

TCPSocket Network::accept(const TCPSocket &src)
{
    int res = 0;
    struct sockaddr clientAddr;
    socklen_t clientSockLen = sizeof(clientAddr);

    res = accept(src._sockfd, reinterpret_cast<struct sockaddr *>(&clientAddr),
        &clientSockLen);
    if (res == -1 || res == 0)
        throw NetworkException("System::Network::accept: Failed to accept "
                               "connection to TCP socket: ");
    return (Network::TCPSocket(res));
}

#endif

void Network::select(socketSetTCP *readfds, socketSetTCP *writefds,
    socketSetTCP *exceptfds, timeoutStruct timeout)
{
    fd_set read_set;
    fd_set write_set;
    fd_set except_set;
    fd_set *read_set_p = NULL;
    fd_set *write_set_p = NULL;
    fd_set *except_set_p = NULL;
    struct timeval *tm = NULL;

    if (timeout.has_value())
        tm = &timeout.value();
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&except_set);

    if (readfds != nullptr && readfds->size() > 0) {
        read_set_p = &read_set;
        for (const TCPSocket *i : *readfds) {
            if (i != nullptr)
                FD_SET(i->_sockfd, read_set_p);
        }
    }
    if (writefds != nullptr && writefds->size() > 0) {
        write_set_p = &write_set;
        for (const TCPSocket *i : *writefds) {
            if (i != nullptr)
                FD_SET(i->_sockfd, write_set_p);
        }
    }
    if (exceptfds != nullptr && exceptfds->size() > 0) {
        except_set_p = &except_set;
        for (const TCPSocket *i : *exceptfds) {
            if (i != nullptr)
                FD_SET(i->_sockfd, except_set_p);
        }
    }
    if (select(FD_SETSIZE, read_set_p, write_set_p, except_set_p, tm) == -1)
        throw NetworkException("Network::select failed");
    if (readfds != nullptr && readfds->size() > 0) {
        for (size_t i = 0; i != readfds->size(); i++) {
            if ((*readfds)[i] != nullptr
                && !FD_ISSET((*readfds)[i]->_sockfd, read_set_p)) {
                readfds->erase((readfds->begin()) + (long) i);
            }
        }
    }
    if (writefds != nullptr && writefds->size() > 0) {
        for (size_t i = 0; i != writefds->size(); i++) {
            if ((*writefds)[i] != nullptr
                && !FD_ISSET((*writefds)[i]->_sockfd, write_set_p)) {
                writefds->erase((writefds->begin()) + (long) i);
            }
        }
    }
    if (exceptfds != nullptr && exceptfds->size() > 0) {
        for (size_t i = 0; i != exceptfds->size(); i++) {
            if ((*exceptfds)[i] != nullptr
                && !FD_ISSET((*exceptfds)[i]->_sockfd, except_set_p)) {
                exceptfds->erase((exceptfds->begin()) + (long) i);
            }
        }
    }
}
