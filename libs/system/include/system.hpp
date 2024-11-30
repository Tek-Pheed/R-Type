/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace System
{
    namespace Network
    {
        class NetworkException;
        class TCPClient;
        class TCPSocket;

        using socketSetTCP = std::vector<TCPSocket *>;
        using timeoutStruct = std::optional<struct timeval>;
        using byteArray = std::vector<uint8_t>;

        void addSocketToSet(const std::vector<System::Network::TCPSocket> &src,
            socketSetTCP &dest);
        void addSocketToSet(
            System::Network::TCPSocket *src, socketSetTCP &dest);
        bool removeSocketInVect(const System::Network::TCPSocket &toRemove,
            std::vector<System::Network::TCPSocket> &vect);
        bool removeSocketInSet(const System::Network::TCPSocket &toRemove,
            socketSetTCP &set);
        std::string decodeString(const byteArray &bytes);
    } // namespace Network
} // namespace System

#endif /* SYSTEM_HPP */
