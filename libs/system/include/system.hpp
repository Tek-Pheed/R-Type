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

#if defined(LINUX)
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
using osSocketType = int;
using s_timeval = struct timeval;
using SOCKADDR_IN = struct sockaddr_in;
#elif defined(WIN32)
    #include <WinSock2.h>
using osSocketType = SOCKET;
typedef long ssize_t;
typedef timeval s_timeval;
#endif

namespace System
{
    namespace Network
    {
        class NetworkException;
        class TCPClient;
        class TCPSocket;

        using socketSetTCP = std::vector<TCPSocket *>;
        using timeoutStruct = std::optional<s_timeval>;
        using byteArray = std::vector<uint8_t>;

        /**
         * @brief Intitialize the Network library (Windows specific).
         * @note Note: This call is not required on Linux and will defined as
         * an empty function.
         */
        void initNetwork(void);

        /**
         * @brief Stop the Network library (Windows specific).
         * @note Note: This call is not required on Linux and will defined as
         * an empty function.
         */
        void stopNetwork(void);

        /**
         * @brief Adds a vector of sockets to a socketSetTCP object.
         *
         * @param src: A vector containing sockets
         * @param dest: The destination socketSetTCP object
         */
        void addSocketToSet(const std::vector<System::Network::TCPSocket> &src,
            socketSetTCP &dest);

        /**
         * @brief Adds a socket pointer to a socketSetTCP object.
         *
         * @param src: A TCPSocket pointer
         * @param dest: The destination socketSetTCP object
         */
        void addSocketToSet(
            System::Network::TCPSocket *src, socketSetTCP &dest);

        /**
         * @brief Remove a socket from a vector of socket.
         * @note Note: This functions remove a socket based on its ID.
         * @param src: A TCPSocket pointer
         * @param dest: The vector of TCPSocket containing the socket
         */
        bool removeSocketInVect(const System::Network::TCPSocket &toRemove,
            std::vector<System::Network::TCPSocket> &vect);

        /**
         * @brief Remove a socket from a socketSetTCP.
         * @note Note: This functions remove a socket based on its ID.
         * @param src: A const reference to a TCPSocket object
         * @param dest: The destination socketSetTCP object
         */
        bool removeSocketInSet(
            const System::Network::TCPSocket &toRemove, socketSetTCP &set);

        /**
         * @brief Decode a std::string from a byteArray object
         *
         * @param bytes: byteArray source object
         * @return std::string The decoded text
         */
        std::string decodeString(const byteArray &bytes);
    } // namespace Network
} // namespace System

#endif /* SYSTEM_HPP */
