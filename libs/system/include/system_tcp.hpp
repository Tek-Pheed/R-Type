/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#ifndef SYSTEM_TCP_HPP
#define SYSTEM_TCP_HPP

#include <cstdint>
#include <exception>
#include <string>
#include <sys/types.h>
#include <vector>

#include "system.hpp"

#if defined(LINUX)
    #include <arpa/inet.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/time.h>
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

        class NetworkException : public std::exception {
          public:
            explicit NetworkException(const std::string &str);
            const char *what() const noexcept override;

          private:
            std::string _content;
        };

        class TCPSocket {
          public:
            enum TCPMode { CONNECT, SERVE };

            explicit TCPSocket(
                uint16_t port, TCPMode mode, std::string address = "");

            TCPSocket(TCPSocket &&) = default;
            TCPSocket(const TCPSocket &) = default;
            TCPSocket &operator=(TCPSocket &&) = default;
            TCPSocket &operator=(const TCPSocket &) = default;
            ~TCPSocket();

            ssize_t send(std::vector<uint8_t> byteSequence);
            std::vector<uint8_t> receive(void);

#if defined(LINUX)
            explicit TCPSocket(int sock_fd);
#elif defined(WIN32)
            explicit TCPSocket(SOCKET _sock);
#endif
            friend TCPSocket accept(const TCPSocket &src);
            friend ssize_t select(std::vector<TCPSocket> readfds,
                std::vector<TCPSocket> writefds,
                std::vector<TCPSocket> exceptfds, struct timeval timeout);

          private:
            TCPMode _mode;

#if defined(LINUX)
          private:
            struct sockaddr_in _sockSettings;
            int _sockfd;

#elif defined(WIN32)
          private:
            SOCKET _sock;
#endif
        };

        TCPSocket accept(const TCPSocket &src);
        ssize_t select(std::vector<TCPSocket> readfds,
            std::vector<TCPSocket> writefds, std::vector<TCPSocket> exceptfds,
            struct timeval timeout);
    } // namespace Network

} // namespace System

void empty(void);

#endif /* SYSTEM_TCP_HPP */
