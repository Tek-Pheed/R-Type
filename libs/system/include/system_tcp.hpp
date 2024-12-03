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
        class TCPSocket : public ASocket {
          public:
            /**
             * @brief Construct a new TCPSocket object
             *
             * @param port: The port which the socket should use
             * @param mode: CONNECT or SERVE
             * @param address: The IP address of the socket (default 0.0.0.0)
             */
            explicit TCPSocket(uint16_t port, Mode mode,
                const std::string &address = "0.0.0.0");

            TCPSocket(TCPSocket &&) = default;
            TCPSocket(const TCPSocket &) = default;
            TCPSocket &operator=(TCPSocket &&) = default;
            TCPSocket &operator=(const TCPSocket &) = default;
            bool operator==(const TCPSocket &);
            bool operator!=(const TCPSocket &);
            ~TCPSocket() override;
            ssize_t sendData(const byteArray &byteSequence) override;
            byteArray receive(void) override;

            explicit TCPSocket(osSocketType _sock);
            friend TCPSocket accept(const TCPSocket &src);
            friend void select(socketSetTCP *readfds, socketSetTCP *writefds,
                socketSetTCP *exceptfds, timeoutStruct timeout);
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
    } // namespace Network

} // namespace System

#endif /* SYSTEM_TCP_HPP */
