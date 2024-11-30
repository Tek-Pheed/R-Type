/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#ifndef SYSTEM_TCP_HPP
#define SYSTEM_TCP_HPP

#include <cstdint>
#include <optional>
#include <string>
#include "system.hpp"

#if defined(LINUX)
    #include <arpa/inet.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
#elif defined(WIN32)
    #include <winbase.h>
    #include <winsock2.h>
#else
    #error "This target is not supported by R-Type libSystem"
#endif

namespace System
{

    namespace Network
    {

#if defined(LINUX)
        using osSocketType = int;
#elif defined(WIN32)
        using osSocketType = SOCKET;
#endif

        class TCPSocket {
          public:
            enum TCPMode { CONNECT, SERVE };

            explicit TCPSocket(uint16_t port, TCPMode mode,
                const std::string &address = "0.0.0.0");

            TCPSocket(TCPSocket &&) = default;
            TCPSocket(const TCPSocket &) = default;
            TCPSocket &operator=(TCPSocket &&) = default;
            TCPSocket &operator=(const TCPSocket &) = default;
            bool operator==(const TCPSocket &);
            bool operator!=(const TCPSocket &);
            ~TCPSocket();

            ssize_t send(const byteArray &byteSequence);
            byteArray receive(void);
            void closeSocket(void);
            bool isOpen(void);
            u_int64_t getUID(void) const;

            explicit TCPSocket(osSocketType _sock);
            friend TCPSocket accept(const TCPSocket &src);
            friend void select(socketSetTCP *readfds, socketSetTCP *writefds,
                socketSetTCP *exceptfds, std::optional<struct timeval> timeout);

          private:
            u_int64_t _uid;
            TCPMode _mode;
            osSocketType _sockfd;
            bool _opened;
#if defined(LINUX)
            struct sockaddr_in _sockSettings;
#endif
        };

        TCPSocket accept(const TCPSocket &src);
        void select(socketSetTCP *readfds = nullptr,
            socketSetTCP *writefds = nullptr,
            socketSetTCP *exceptfds = nullptr,
            timeoutStruct timeout = std::nullopt);
    } // namespace Network

} // namespace System

void empty(void);

#endif /* SYSTEM_TCP_HPP */
