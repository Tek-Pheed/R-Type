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
#include <optional>
#include <string>
#include <vector>

#if defined(LINUX)
    #include <arpa/inet.h>
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
#elif defined(WIN32)
        using osSocketType = SOCKET;
#endif

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
                uint16_t port, TCPMode mode, const std::string &address = "");

            TCPSocket(TCPSocket &&) = default;
            TCPSocket(const TCPSocket &) = default;
            TCPSocket &operator=(TCPSocket &&) = default;
            TCPSocket &operator=(const TCPSocket &) = default;
            ~TCPSocket();

            ssize_t send(const std::vector<uint8_t> &byteSequence);
            std::vector<uint8_t> receive(void);

            explicit TCPSocket(osSocketType _sock);
            friend TCPSocket accept(const TCPSocket &src);
            friend int select(std::optional<std::vector<TCPSocket>> &readfds,
                std::optional<std::vector<TCPSocket>> &writefds,
                std::optional<std::vector<TCPSocket>> &exceptfds,
                std::optional<struct timeval> &timeout);

          private:
            TCPMode _mode;
            osSocketType _sockfd;
#if defined(LINUX)
            struct sockaddr_in _sockSettings;
#endif
        };

        TCPSocket accept(const TCPSocket &src);
        int select(std::optional<std::vector<TCPSocket>> &readfds,
            std::optional<std::vector<TCPSocket>> &writefds,
            std::optional<std::vector<TCPSocket>> &exceptfds,
            std::optional<struct timeval> &timeout);
    } // namespace Network

} // namespace System

void empty(void);

#endif /* SYSTEM_TCP_HPP */
