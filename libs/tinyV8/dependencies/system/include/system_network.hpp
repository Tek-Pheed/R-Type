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
    #include <netdb.h>
    #include <netinet/in.h>

    #define INVALID_SOCKET -1
    #define SOCKET_ERROR   -1
using osSocketType = int;
typedef struct timeval s_timeval;
using osSocketType = int;
using s_timeval = struct timeval;
using SOCKADDR_IN = struct sockaddr_in;

#elif defined(WIN32)
    #include <WinSock2.h>
    #include <Ws2tcpip.h>
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

        using timeoutStruct = std::optional<s_timeval>;
        using byteArray = std::vector<uint8_t>;

        /**
         * @brief Socket Interface
         */
        class ISocket {
          public:
            virtual ~ISocket() = default;

            /**
             * @brief Mode for creating the socket
             * @li - Choose mode CONNECT to connect to a socket.
             * @li - Choose mode SERVE, if this socket
             * needs to receive connections from clients
             */
            enum Mode { CONNECT, SERVE };
            enum Type { TCP, UDP };

            /**
             * @brief Send data through the socket
             *
             * @param byteSequence: A byte sequence object (type byteArray)
             * @return: ssize_t The number of bytes written in the socket. This
             * might be less than the size of the byte array, in which case you
             * should call this function later with the rest of the data to
             * send.
             */
            virtual ssize_t sendData(const byteArray &byteSequence) = 0;

            /**
             * @brief Receive data through the socket
             * @note Note: This call could block if the socket is invalid or if
             * there is no data available at the moment. You probably should
             * use select before this function.
             * @return byteArray: A byte sequence object representing all bytes
             * read from the socket
             */
            virtual byteArray receive(void) = 0;

            /**
             * @brief Close the socket.
             * @note Note: the socket should not be used after this call
             */
            virtual void closeSocket(void) = 0;

            /**
             * @brief Get the OS-native socket handle
             * @note Note: this call should probably not be use directly
             */
            virtual osSocketType getHandle(void) const = 0;

            /**
             * @brief Return true if the socket is connected and opened
             *
             * @return true: The socket is connected and available
             * @return false: The socket is disconnected and should not be used
             * (or it will throw)
             */
            virtual bool isOpen(void) const = 0;

            /**
             * @brief Get the unique identitifer of this socket
             *
             * @return uint64_t: Unique identifier of the socket
             */
            virtual uint64_t getUID(void) const = 0;


            /**
             * @brief Return the type of the socket
             *
             * @return Type: Type of the socket
             */
            virtual Type getType() const = 0;
        };

        class ASocket : public ISocket {
          public:
            bool operator==(const ASocket &);
            bool operator!=(const ASocket &);

            virtual ssize_t sendData(
                const byteArray &byteSequence) override = 0;
            virtual byteArray receive(void) override = 0;

            void closeSocket(void) override;
            bool isOpen(void) const override;
            uint64_t getUID(void) const override;
            osSocketType getHandle(void) const override;
            Type getType() const override;

          protected:
            uint64_t _uid;
            Mode _mode;
            osSocketType _sockfd;
            bool _opened;
            SOCKADDR_IN _sockSettings;
            Type _type;
        };

        class TCPClient;
        class TCPSocket;
        class UDPSocket;

        using socketSetTCP = std::vector<TCPSocket *>;
        using socketSetUDP = std::vector<UDPSocket *>;
        using socketSetGeneric = std::vector<ISocket *>;

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
         * @brief Decode a std::string from a byteArray object
         *
         * @param bytes: byteArray source object
         * @return std::string The decoded text
         */
        std::string decodeString(const byteArray &bytes);

        /**
         * @brief Encode a string into a byteArray object
         *
         * @param str: std::string source object
         * @return byteArray object
         */
        byteArray encodeString(const std::string &str);

        /**
         * @brief select applied to TCPSocket objects.
         * @note Note: This function modifies the sets passed as argument.
         * @param readfds: A socketSetGeneric object which conatains all sockets
         * that should be watch to see if they are ready for reading.
         * @param writefds: A socketSetGeneric object which conatains all sockets
         * that should be watch to see if they are ready for writing.
         * @param exceptfds: A socketSetGeneric object which conatains all sockets
         * that should be watch for exceptionnal events.
         * @param timeout The timeout argument is a timeoutStruct that
         * specifies the interval that select() should block waiting for a file
         * descriptor to become ready. If the argument is unsepcified, select
         * will block indefinitely waiting for a socket to become ready.
         */
        void select(socketSetGeneric *readfds = nullptr,
            socketSetGeneric *writefds = nullptr,
            socketSetGeneric *exceptfds = nullptr,
            timeoutStruct timeout = std::nullopt);

    } // namespace Network
} // namespace System

#endif /* SYSTEM_NETWORK_HPP */
