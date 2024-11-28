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
#include <bits/types/struct_timeval.h>

using namespace System;
using namespace Network;

#if defined(LINUX)
TCPSocket::TCPSocket(uint16_t port, TCPMode mode, std::string address)
{
    int res = 0;
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

TCPSocket::TCPSocket(int sock_fd)
{
    this->_sockfd = sock_fd;
}

TCPSocket::~TCPSocket()
{
    if (this->_sockfd != -1)
        close(this->_sockfd);
}

ssize_t TCPSocket::send(std::vector<uint8_t> byteSequence)
{
    ssize_t writtenBytes = 0;
    size_t len = byteSequence.size();
    uint8_t *buff = (uint8_t *) calloc(byteSequence.size(), sizeof(uint8_t));

    for (size_t i = 0; i < len; i++) {
        buff[i] = byteSequence[i];
    }
    writtenBytes = write(this->_sockfd, (void *) buff, len);
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
    uint8_t *buff = (uint8_t *) calloc(len, sizeof(uint8_t));
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

// TODO: Encapsulate select with fd_set FD_ISSET ...

// ssize_t Network::select(std::vector<TCPSocket> readfds,
//     std::vector<TCPSocket> writefds, std::vector<TCPSocket> exceptfds,
//     struct timeval timeout)
// {
//     select(FD_SETSIZE, fd_set *__restrict readfds, fd_set *__restrict writefds, fd_set *__restrict exceptfds, struct timeval *__restrict timeout)

// }

#endif
