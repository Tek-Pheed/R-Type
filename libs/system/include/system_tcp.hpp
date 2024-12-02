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
#include "system_network.hpp"

namespace System
{

    namespace Network
    {

        /**
         * @brief Implementation of TCP Socket for Windows/Linux
         * @details This class absatracts the low level details about sockets,
         * while still providing a similar API to Linux sockets.
         */
        class TCPSocket {
          public:
            /**
             * @brief Mode for creating the TCP socket
             * @li - Choose mode CONNECT to connect to another TCP socket.
             * @li - Choose mode SERVE, if this socket
             * needs to receive connections from clients
             */
            enum TCPMode { CONNECT, SERVE };

            /**
             * @brief Construct a new TCPSocket object
             *
             * @param port: The port which the socket should use
             * @param mode: CONNECT or SERVE
             * @param address: The IP address of the socket (default 0.0.0.0)
             */
            explicit TCPSocket(uint16_t port, TCPMode mode,
                const std::string &address = "0.0.0.0");

            TCPSocket(TCPSocket &&) = default;
            TCPSocket(const TCPSocket &) = default;
            TCPSocket &operator=(TCPSocket &&) = default;
            TCPSocket &operator=(const TCPSocket &) = default;
            bool operator==(const TCPSocket &);
            bool operator!=(const TCPSocket &);
            ~TCPSocket();

            /**
             * @brief Send data through the socket
             *
             * @param byteSequence: A byte sequence object (type byteArray)
             * @return: ssize_t The number of bytes written in the socket. This
             * might be less than the size of the byte array, in which case you
             * should call this function later with the rest of the data to
             * send.
             */
            ssize_t sendData(const byteArray &byteSequence);

            /**
             * @brief Receive data through the socket
             * @note Note: This call could block if the socket is invalid or if
             * there is no data available at the moment. You probably should
             * use select before this function.
             * @return byteArray: A byte sequence object representing all bytes
             * read from the socket
             */
            byteArray receive(void);

            /**
             * @brief Close the socket.
             * @note Note: the socket should not be used after this call
             */
            void closeSocket(void);

            /**
             * @brief Return true if the socket is connected and opened
             *
             * @return true: The socket is connected and available
             * @return false: The socket is disconnected and should not be used
             * (or it will throw)
             */
            bool isOpen(void);

            /**
             * @brief Get the unique identitifer of this socket
             *
             * @return uint64_t: Unique identifier of the socket
             */
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

        /**
         * @brief accept applied to TCPSocket objects.
         *
         * @param src The socket on which the accept syscall should be applied
         * @return TCPSocket: On success, this function returns a TCPSocket
         * object for the accepted socket
         */
        TCPSocket accept(const TCPSocket &src);

        /**
         * @brief select applied to TCPSocket objects.
         * @note Note: This function modifies the sets passed as argument.
         * @param readfds: A socketSetTCP object which conatains all sockets
         * that should be watch to see if they are ready for reading.
         * @param writefds: A socketSetTCP object which conatains all sockets
         * that should be watch to see if they are ready for writing.
         * @param exceptfds: A socketSetTCP object which conatains all sockets
         * that should be watch for exceptionnal events.
         * @param timeout The timeout argument is a timeoutStruct that
         * specifies the interval that select() should block waiting for a file
         * descriptor to become ready. If the argument is unsepcified, select
         * will block indefinitely waiting for a socket to become ready.
         */
        void select(socketSetTCP *readfds = nullptr,
            socketSetTCP *writefds = nullptr,
            socketSetTCP *exceptfds = nullptr,
            timeoutStruct timeout = std::nullopt);
    } // namespace Network

} // namespace System

void empty(void);

#endif /* SYSTEM_TCP_HPP */
