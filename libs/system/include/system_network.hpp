/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#ifndef SYSTEM_NETWORK_HPP
#define SYSTEM_NETWORK_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#if defined(LINUX)
    #include <arpa/inet.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>

    #define INVALID_SOCKET -1
    #define SOCKET_ERROR   -1
    using osSocketType = int;
    typedef struct timeval s_timeval;
    using osSocketType = int;
    using s_timeval = struct timeval;
    using SOCKADDR_IN = struct sockaddr_in;

#elif defined(WIN32)
    #include <WinSock2.h>
    using osSocketType = SOCKET;
    typedef long ssize_t;
    typedef timeval s_timeval;
    typedef int socklen_t;

#else
    #error "This target is not supported by R-Type libSystem"
#endif

namespace System
{
    namespace Network
    {
        /**
         * @brief The Network Exception class.
         *
         *
         * @note Node: This exception hold a string containing a network error
         * raised by the OS or the library.
         */
        class NetworkException : public std::exception {
          public:
            explicit NetworkException(const std::string &str);
            const char *what() const noexcept override;

          private:
            std::string _content;
        };

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

#endif /* SYSTEM_NETWORK_HPP */
