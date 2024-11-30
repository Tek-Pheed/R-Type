/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_tcp
*/

#include "system_tcp.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace System;
using namespace Network;

#if defined(LINUX)
TCPSocket::TCPSocket(uint16_t port, TCPMode mode, const std::string &address)
{
    int res;
    this->_sockSettings.sin_family = AF_INET;
    this->_sockSettings.sin_port = htons(port);
    this->_sockfd = socket(_sockSettings.sin_family, SOCK_STREAM, 0);
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

    } else if (mode == TCPMode::CONNECT) {
        this->_sockSettings.sin_addr.s_addr = inet_addr(address.c_str());
        res = connect(this->_sockfd,
            reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
            sizeof(this->_sockSettings));
        if (res == -1)
            throw NetworkException(
                "System::Network::TCPSocket: Failed to connect");
    }
}

TCPSocket::TCPSocket(osSocketType sock_fd)
{
    this->_sockfd = sock_fd;
}

TCPSocket::~TCPSocket()
{
    if (this->_sockfd != -1)
        close(this->_sockfd);
}

ssize_t TCPSocket::send(const std::vector<uint8_t> &byteSequence)
{
    ssize_t writtenBytes = 0;
    size_t len = byteSequence.size();
    uint8_t *buff = reinterpret_cast<uint8_t *>(
        calloc(byteSequence.size(), sizeof(uint8_t)));

    for (size_t i = 0; i < len; i++) {
        buff[i] = byteSequence[i];
    }
    writtenBytes = write(this->_sockfd, reinterpret_cast<void *>(buff), len);
    if (writtenBytes == -1)
        throw NetworkException("System::Network::TCPSocket: Failed to send");
    free(buff);
    return (writtenBytes);
}

std::vector<uint8_t> TCPSocket::receive(void)
{
    std::vector<uint8_t> vect;
    size_t len = 0;

    ioctl(this->_sockfd, FIONREAD, &len);
    if (len == 0) {
        return (vect);
    }
    uint8_t *buff = reinterpret_cast<uint8_t *>(calloc(len, sizeof(uint8_t)));
    if (read(this->_sockfd, &buff, len) == -1)
        throw NetworkException("System::Network::TCPSocket: Failed to read");
    for (size_t i = 0; i < len; i++)
        vect[i] = buff[i];
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
                               "connection to TCP socket");
    return (Network::TCPSocket(res));
}

int Network::select(std::optional<std::vector<TCPSocket>> &readfds,
    std::optional<std::vector<TCPSocket>> &writefds,
    std::optional<std::vector<TCPSocket>> &exceptfds,
    std::optional<struct timeval> &timeout)
{
    int ret = 0;
    fd_set read_set;
    fd_set write_set;
    fd_set except_set;
    struct timeval *tm = NULL;
    size_t total_fds = 0;

    if (timeout.has_value())
        tm = &timeout.value();
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&except_set);

    if (readfds.has_value()) {
        for (const TCPSocket &i : readfds.value()) {
            FD_SET(i._sockfd, &read_set);
            total_fds++;
        }
    }
    if (writefds.has_value()) {
        for (const TCPSocket &i : writefds.value()) {
            FD_SET(i._sockfd, &write_set);
            total_fds++;
        }
    }
    if (exceptfds.has_value()) {
        for (const TCPSocket &i : exceptfds.value()) {
            FD_SET(i._sockfd, &except_set);
            total_fds++;
        }
    }
    ret = select((int) total_fds, &read_set, &write_set, &except_set, tm);
    if (readfds.has_value()) {
        for (size_t i = 0; i != readfds.value().size(); i++) {
            if (!FD_ISSET(readfds.value()[i]._sockfd, &read_set)) {
                readfds.value().erase((readfds.value().begin()) + (long) i);
            }
        }
    }
    if (writefds.has_value()) {
        for (size_t i = 0; i != writefds.value().size(); i++) {
            if (!FD_ISSET(writefds.value()[i]._sockfd, &read_set)) {
                writefds.value().erase((writefds.value().begin()) + (long) i);
            }
        }
    }
    if (exceptfds.has_value()) {
        for (size_t i = 0; i != exceptfds.value().size(); i++) {
            if (!FD_ISSET(exceptfds.value()[i]._sockfd, &read_set)) {
                exceptfds.value().erase(
                    (exceptfds.value().begin()) + (long) i);
            }
        }
    }
    return (ret);
}

#endif
