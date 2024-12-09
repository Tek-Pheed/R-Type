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

void UDPSocket::initSocket(
    uint16_t port, Mode mode, const std::string &address)
{
    this->_sockSettings.sin_family = AF_INET;
    this->_sockSettings.sin_port = htons(port);
    this->_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    this->_uid = udpSockID;
    this->_mode = mode;
    if (this->_sockfd == INVALID_SOCKET)
        throw NetworkException(
            "System::Network::UDPSocket: Failed to create socket");

    if (address == "0.0.0.0") {
        _sockSettings.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        if (inet_pton(AF_INET, address.c_str(), &this->_sockSettings.sin_addr)
            != 1) {
            throw NetworkException("System::Network::UDPSocket: Invalid "
                                   "address/ Address not supported");
        }
    }
    if (_mode == SERVE) {
        int res;
        res = bind(this->_sockfd,
            reinterpret_cast<struct sockaddr *>(&this->_sockSettings),
            sizeof(this->_sockSettings));
        if (res == SOCKET_ERROR)
            throw NetworkException(
                "System::Network::UDPSocket: Failed to bind socket");
    }
    this->_opened = true;
}

UDPSocket::UDPSocket()
{
    udpSockID++;
    this->_type = System::Network::ISocket::UDP;
    this->_uid = udpSockID;
    this->_opened = false;
}

UDPSocket::UDPSocket(uint16_t port, Mode mode, const std::string &address)
{
    udpSockID++;
    this->_type = System::Network::ISocket::UDP;
    this->_uid = udpSockID;
    this->_opened = false;
    this->_mode = mode;
    this->initSocket(port, mode, address);
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

ssize_t UDPSocket::sendDataTo(
    const byteArray &byteSequence, const std::string &address, uint16_t port)
{
    SOCKADDR_IN sockin;
    ssize_t writtenBytes = 0;
    size_t len = byteSequence.size();
    uint8_t *buff = new uint8_t[len]();

    if (buff == NULL)
        throw std::runtime_error("System::Network::UDPSocket::sendDataTo: "
                                 "Failed to allocate send buffer");
    sockin.sin_family = AF_INET;
    sockin.sin_port = htons(port);
    if (inet_pton(AF_INET, address.c_str(), &sockin.sin_addr) != 1) {
        delete[] buff;
        throw std::runtime_error("System::Network::UDPSocket: Invalid "
                                 "address/ Address not supported");
    }
    for (size_t i = 0; i < len; i++) {
        buff[i] = byteSequence[i];
    }
    writtenBytes = sendto(this->_sockfd, reinterpret_cast<const char *>(buff),
        len, 0, reinterpret_cast<const sockaddr *>(&sockin), sizeof(sockin));
    this->_opened = (writtenBytes > 0);
    delete[] buff;
    if (writtenBytes == SOCKET_ERROR)
        throw NetworkException(
            "System::Network::UDPSocket::sendDataTo: Failed to send");
    return (writtenBytes);
}

UDPSocket::UDPSocket(osSocketType sock_fd)
{
    udpSockID++;
    this->_type = System::Network::ISocket::UDP;
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

#if defined(WIN32)
    u_long len = 0;
    ioctlsocket(this->_sockfd, FIONREAD, &len);
#elif defined(LINUX)
    size_t len = 0;
    ioctl(this->_sockfd, FIONREAD, &len);
#endif

    if (len == 0) {
        return (vect);
    }
    uint8_t *buff = new uint8_t[len]();
#if defined(LINUX)
    if (buff == nullptr)
        throw std::runtime_error("System::Network::UDPSocket::receive: Unable "
                                 "to allocate receive buffer");
    socklen_t slen = sizeof(this->_sockSettings);
    ret = recvfrom(this->_sockfd, buff, len, 0,
        reinterpret_cast<sockaddr *>(&this->_sockSettings), &slen);

#elif defined(WIN32)
    int slen = sizeof(this->_sockSettings);
    ret = recvfrom(this->_sockfd, reinterpret_cast<char *>(buff), len, 0,
        reinterpret_cast<sockaddr *>(&this->_sockSettings), &slen);
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

byteArray UDPSocket::receiveFrom(std::string &address, uint16_t &port)
{
    ssize_t ret = 0;
    byteArray vect;
    SOCKADDR_IN client_addr;

#if defined(WIN32)
    u_long len = 0;
    ioctlsocket(this->_sockfd, FIONREAD, &len);
#elif defined(LINUX)
    size_t len = 0;
    ioctl(this->_sockfd, FIONREAD, &len);
#endif

    if (len == 0) {
        return (vect);
    }
    uint8_t *buff = new uint8_t[len]();
#if defined(LINUX)
    if (buff == nullptr)
        throw std::runtime_error(
            "System::Network::UDPSocket::receiveFrom: Unable "
            "to allocate receive buffer");
    socklen_t slen = sizeof(client_addr);
    ret = recvfrom(this->_sockfd, buff, len, 0,
        reinterpret_cast<sockaddr *>(&client_addr), &slen);

#elif defined(WIN32)
    int slen = sizeof(client_addr);
    ret = recvfrom(this->_sockfd, reinterpret_cast<char *>(buff), len, 0,
        reinterpret_cast<sockaddr *>(&client_addr), &slen);
#endif
    if (ret == SOCKET_ERROR) {
        delete[] buff;
        throw NetworkException(
            "System::Network::UDPSocket::receiveFrom: Failed to read");
    }
    const char *addr = inet_ntoa(client_addr.sin_addr);
    port = ntohs(client_addr.sin_port);
    if (addr == NULL) {
        delete[] buff;
        throw NetworkException("System::Network::UDPSocket::receiveFrom: "
                               "Failed to get sender address");
    }
    address = addr;
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
    const std::vector<UDPSocket> &src, socketSetGeneric &dest)
{
    for (size_t i = 0; i != src.size(); i++) {
        auto *a = const_cast<UDPSocket *>(&(src.at(i)));
        dest.emplace_back(a);
    }
}

void System::Network::addSocketToSet(UDPSocket *src, socketSetGeneric &dest)
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
