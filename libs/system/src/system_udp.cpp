/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_udp
*/

#include "system_udp.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include "system_network.hpp"

using namespace System;
using namespace Network;

static uint64_t udpSockID = 0;

UDPSocket::UDPSocket(uint16_t port, const std::string &address)
{
    udpSockID++;
    int res;
    this->_opened = false;
    this->_sockSettings.sin_family = AF_INET;
    this->_sockSettings.sin_port = htons(port);
    this->_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    this->_uid = udpSockID;
    if (this->_sockfd == INVALID_SOCKET)
        throw NetworkException(
            "System::Network::UDPSocket: Failed to create socket");

    if (address == "0.0.0.0") {
        _sockSettings.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        this->_sockSettings.sin_addr.s_addr = inet_addr(address.c_str());
    }
    res = bind(this->_sockfd,
        reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
        sizeof(this->_sockSettings));
    if (res == SOCKET_ERROR)
        throw NetworkException(
            "System::Network::UDPSocket: Failed to bind socket");
}

ssize_t UDPSocket::sendData(const byteArray &byteSequence)
{
    ssize_t writtenBytes = 0;
    size_t len = byteSequence.size();
    uint8_t *buff = new uint8_t[len]();

    if (buff == NULL)
        throw std::runtime_error("System::Network::UDPSocket::sendData: "
                                 "Failed to allocate send buffer");
    for (size_t i = 0; i < len; i++) {
        buff[i] = byteSequence[i];
    }
    writtenBytes = sendto(this->_sockfd, reinterpret_cast<const char *>(buff),
        len, 0, reinterpret_cast<const sockaddr *>(&this->_sockSettings),
        sizeof(this->_sockSettings));
    this->_opened = (writtenBytes > 0);
    delete[] buff;
    if (writtenBytes == SOCKET_ERROR)
        throw NetworkException(
            "System::Network::UDPSocket::sendData: Failed to send");
    return (writtenBytes);
}

UDPSocket::UDPSocket(osSocketType sock_fd)
{
    udpSockID++;
    this->_opened = true;
    this->_sockfd = sock_fd;
    this->_uid = udpSockID;
    this->_mode = CONNECT;
}

UDPSocket::~UDPSocket()
{
}

byteArray UDPSocket::receive(void)
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
        socklen_t slen = sizeof(this->_sockSettings);
        ret = recvfrom(this->_sockfd, &dummy, len, 0,
            reinterpret_cast<sockaddr *>(&this->_sockSettings), &slen);
#elif defined(WIN32)
        int slen = sizeof(this->_sockSettings);
        ret = recvfrom(this->_sockfd, reinterpret_cast<char *>(&dummy), len, 0,
            reinterpret_cast<sockaddr *>(&this->_sockSettings), &slen);
        if (this->_opened && ret == SOCKET_ERROR) {
            this->_opened = (ret > 0);
            return (vect);
        }
#endif
        this->_opened = (ret > 0);
        if (ret == SOCKET_ERROR)
            throw NetworkException(
                "System::Network::UDPSocket::receive: Failed to read");
        return (vect);
    }
    uint8_t *buff = new uint8_t[len]();
#if defined(LINUX)
    if (buff == nullptr)
        throw std::runtime_error("System::Network::UDPSocket::receive: Unable "
                                 "to allocate receive buffer");
    socklen_t slen = sizeof(this->_sockSettings);
    ret = recvfrom(this->_sockfd, buff, len, 0, reinterpret_cast<sockaddr *>(&this->_sockSettings), &slen);

#elif defined(WIN32)
    int slen = sizeof(this->_sockSettings);
    ret = recvfrom(this->_sockfd,  reinterpret_cast<char *>(buff), len, 0, reinterpret_cast<sockaddr *>(&this->_sockSettings), &slen);
#endif
    if (ret == SOCKET_ERROR) {
        delete[] buff;
        throw NetworkException(
            "System::Network::UDPSocket::receive: Failed to read");
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

void System::Network::addSocketToSet(
    const std::vector<UDPSocket> &src, socketSetUDP &dest)
{
    for (size_t i = 0; i != src.size(); i++) {
        auto *a = const_cast<UDPSocket *>(&(src.at(i)));
        dest.emplace_back(a);
    }
}

void System::Network::addSocketToSet(UDPSocket *src, socketSetUDP &dest)
{
    dest.emplace_back(src);
}

bool System::Network::removeSocketInVect(
    const System::Network::UDPSocket &toRemove,
    std::vector<System::Network::UDPSocket> &vect)
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
    const System::Network::UDPSocket &toRemove,
    System::Network::socketSetUDP &set)
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
