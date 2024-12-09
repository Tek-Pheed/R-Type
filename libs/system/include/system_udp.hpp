/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_dp
*/

#ifndef SYSTEM_UDP_HPP
#define SYSTEM_UDP_HPP

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
        class UDPSocket : public ASocket {
          public:
            /**
             * @brief Construct a new UDPSocket object
             *
             * @param port: The port which the socket should use
             * @param mode: CONNECT or SERVE
             * @param address: The IP address of the socket (default 0.0.0.0)
             */
            explicit UDPSocket(
                uint16_t port, Mode mode = SERVE, const std::string &address = "0.0.0.0");

            explicit UDPSocket();

            void initSocket(uint16_t port, Mode mode = SERVE,
                const std::string &address = "0.0.0.0");

            UDPSocket(UDPSocket &&) = default;
            UDPSocket(const UDPSocket &) = default;
            UDPSocket &operator=(UDPSocket &&) = default;
            UDPSocket &operator=(const UDPSocket &) = default;
            bool operator==(const UDPSocket &);
            bool operator!=(const UDPSocket &);
            ~UDPSocket() override;

            /**
             * @brief Send data through the socket
             *
             * @param byteSequence: A byte sequence object (type byteArray)
             * @return: ssize_t The number of bytes written in the socket. This
             * might be less than the size of the byte array, in which case you
             * should call this function later with the rest of the data to
             * send.
             */
            ssize_t sendData(const byteArray &byteSequence) override;

            /**
             * @brief Send data through the socket, to a specific client
             *
             * @param byteSequence: A byte sequence object (type byteArray)
             * @param address: The address of the client to send the data to
             * @param port: The port of the client to send the data to
             * @return: ssize_t The number of bytes written in the socket. This
             * might be less than the size of the byte array, in which case you
             * should call this function later with the rest of the data to
             * send.
             */
            ssize_t sendDataTo(const byteArray &byteSequence,
                const std::string &address, uint16_t);

            /**
             * @brief Receive data through the socket
             * @note Note: This call could block if the socket is invalid or if
             * there is no data available at the moment. You probably should
             * use select before this function.
             * @return byteArray: A byte sequence object representing all bytes
             * read from the socket
             */
            byteArray receive(void) override;

            /**
             * @brief Receive data through the socket, set the address argument
             * to the address of the client who sent the packet
             *
             * @param address: The address of the sender will be placed in this
             * string
             * @note Note: This call could block if the socket is invalid or if
             * there is no data available at the moment. You probably should
             * use select before this function.
             * @return byteArray: A byte sequence object representing all bytes
             * read from the socket
             */
            byteArray receiveFrom(std::string &address, uint16_t &port);

            explicit UDPSocket(osSocketType _sock);
            // friend UDPSocket accept(const UDPSocket &src);
            // friend void select(socketSet *readfds, socketSet *writefds,
            //     socketSet *exceptfds, timeoutStruct timeout);
        };

        /**
         * @brief Adds a vector of sockets to a socketSetGeneric object.
         *
         * @param src: A vector containing sockets
         * @param dest: The destination socketSetGeneric object
         */
        void addSocketToSet(const std::vector<System::Network::UDPSocket> &src,
            socketSetGeneric &dest);

        /**
         * @brief Adds a socket pointer to a socketSetGeneric object.
         *
         * @param src: A TCPSocket pointer
         * @param dest: The destination socketSetGeneric object
         */
        void addSocketToSet(
            System::Network::UDPSocket *src, socketSetGeneric &dest);

        /**
         * @brief Remove a socket from a vector of socket.
         * @note Note: This functions remove a socket based on its ID.
         * @param src: A TCPSocket pointer
         * @param dest: The vector of TCPSocket containing the socket
         */
        bool removeSocketInVect(const System::Network::UDPSocket &toRemove,
            std::vector<System::Network::UDPSocket> &vect);

        /**
         * @brief Remove a socket from a socketSetGeneric.
         * @note Note: This functions remove a socket based on its ID.
         * @param src: A const reference to a TCPSocket object
         * @param dest: The destination socketSetGeneric object
         */
        bool removeSocketInSet(
            const System::Network::UDPSocket &toRemove, socketSetGeneric &set);
    } // namespace Network

} // namespace System

#endif