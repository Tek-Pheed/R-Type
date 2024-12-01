/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#ifndef SYSTEM_TCP_HPP
#define SYSTEM_TCP_HPP

#include <cstdint>
#include <ctype.h>
#include <optional>
#include <string>
#include "system.hpp"

#if defined(LINUX)
    #include <arpa/inet.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
#elif defined(WIN32)
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
        typedef struct timeval s_timeval;
#elif defined(WIN32)
        using osSocketType = SOCKET;
        typedef long ssize_t;
        typedef timeval s_timeval;
        typedef int socklen_t;
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

            ssize_t sendData(const byteArray &byteSequence);
            byteArray receive(void);
            void closeSocket(void);
            bool isOpen(void);
            uint64_t getUID(void) const;

            explicit TCPSocket(osSocketType _sock);
            friend TCPSocket accept(const TCPSocket &src);
            friend void select(socketSetTCP *readfds, socketSetTCP *writefds,
                socketSetTCP *exceptfds, timeoutStruct timeout);

          private:
            uint64_t _uid;
            TCPMode _mode;
            osSocketType _sockfd;
            bool _opened;
            SOCKADDR_IN _sockSettings;
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
