/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

/**
 * @file Game engine networking classes.
 */

#ifndef TINY_V8_NETWORKING
#define TINY_V8_NETWORKING

#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include "Engine.hpp"
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"
#include <condition_variable>
#include <unordered_map>

template <typename T> std::string getString(T arg)
{
    return (std::to_string(arg));
}

template <> inline std::string getString(const char *arg)
{
    return (std::string(arg));
}

template <> inline std::string getString(const std::string &arg)
{
    return (arg);
}

template <> inline std::string getString(std::string &arg)
{
    return (arg);
}

template <> inline std::string getString(std::string arg)
{
    return (arg);
}

namespace Engine
{
    namespace Events
    {
        // Event when as a client you are connecting to the server, no
        // argument
        constexpr auto EVENT_ConnectedToServer{"onConnectedToServer"};

        // Event when as a client you lost the connection to the server, no
        // argument
        constexpr auto EVENT_DisconnectedFromServer{"onDisconnectedToServer"};

        // Event when as a server you get a new connection, no argument
        constexpr auto EVENT_OnServerNewClient{"onServerNewClient"};

        // Event when as a server a client disconnects, ARG: ssize_t (client
        // ID)
        constexpr auto EVENT_OnServerLostClient{"onServerNewClient"};

        // Event network data received on a socket, no argument
        constexpr auto EVENT_OnDataReceived {"onDataReceived"};

    }; // namespace Events

    /**
     * @brief Interface for implementing packet-specific operations.

     * The functions in this interface should be defined by the user by
     creating a derived class.
     * This class is used for separaring/extracting packets, according to your
     protocol needs.
     * This class should also provide a function to identify clients when they
     connect to the server.
     */
    class IPacketManager {
      public:
        virtual ~IPacketManager() = default;

        /**
         * @brief Extracts all packets from the given buffer.

         * This functions splits all packets from a buffer of bytes, and sets
         the resultIndexEnd to the index of end the last whole packet parsed.
         * @param bytes: A sequence of byte read from a socket.
         * @param resultIndexEnd: You should set this to the index of the last
         complete packet parsed.
         * @return std::vector<std::string>: A vector of decoded packets.
         */
        virtual std::vector<std::string> splitPackets(
            const System::Network::byteArray &bytes,
            size_t &resultIndexEnd) = 0;

        /**
         * @brief Identifies a client give a received buffer.

         * This function should extract the client ID from the stream of byte.
         * @param bytes: A sequence of byte read from a socket.
         * @return ssize_t: The client ID, -1 if undefined.
         */
        virtual ssize_t identifyClient(
            const System::Network::byteArray &bytes) = 0;

        /**
         * @brief Serialize a string

         * Serialize a string for sending it through a socket.
         * @param str: The string to serialize.
         * @param out: The output buffer.
         */
        virtual void serializeString(
            const std::string &str, std::ostream &out) = 0;
    };
    namespace Feature
    {
        class NetworkingManager : public AEngineFeature {
            static size_t constexpr DEFAULT_TCP_PORT = 8081;
            static size_t constexpr DEFAULT_UDP_PORT = 8082;
            struct NetClient {
              public:
                System::Network::TCPSocket tcpSocket;
                std::string ip;
                uint16_t port;
                System::Network::byteArray readBufferTCP;
                System::Network::byteArray writeBufferTCP;
                System::Network::byteArray readBufferUDP;
                System::Network::byteArray writeBufferUDP;
                bool isReady;
                bool isDisconnected;

                NetClient()
                    : tcpSocket(System::Network::TCPSocket()),
                      port(uint16_t()),
                      readBufferTCP(System::Network::byteArray()),
                      writeBufferTCP(System::Network::byteArray()),
                      readBufferUDP(System::Network::byteArray()),
                      writeBufferUDP(System::Network::byteArray()),
                      isReady(false), isDisconnected(false) {};
            };

          public:
            explicit NetworkingManager(Core &engineRef);
            ~NetworkingManager();

            /**
             * @brief Sets the TCP port which the class will use.

             * @note: Calling this after setupClient or setupServer will have
             no effect.
             * @param port: The port to use.
             */
            void setTCPPort(size_t port);

            /**
             * @brief Sets the UDP port which the class will use.

             * @note: Calling this after setupClient or setupServer will have
             no effect.
             * @param port: The port to use.
            */
            void setUDPPort(size_t port);

            /**
             * @brief Returnn defined TCP port.

             * @return size_t: TCP port.
             */
            size_t getTCPPort(void) const;

            /**
             * @brief Returnn defined UDP port.

             * @return size_t: UDP port.
            */
            size_t getUDPPort(void) const;

            /**
             * @brief Setup a server, create all necessary sockets and threads.

             * @tparam PacketManager: A packetManager class. (Should be derived
             from IPacketManager).
             * @param TCP_port: TCP socket port.
             * @param UDP_port: UDP socket port.
             */
            template <class PacketManager>
            void setupServer(uint16_t TCP_port, uint16_t UDP_port)
            {
                _pacMan = std::make_unique<PacketManager>();
                _isServer = true;
                _SocketTCP.initSocket(
                    TCP_port, System::Network::ISocket::SERVE);
                _SocketUDP.initSocket(UDP_port);
                std::cout << "ENGINE: Running server on port TCP:"
                          << std::to_string(TCP_port)
                          << ", UDP:" << std::to_string(UDP_port) << std::endl;

                std::thread(&NetworkingManager::runReadThread, this).detach();
                std::thread(&NetworkingManager::runWriteThread, this).detach();
                std::thread(&NetworkingManager::runConnectThread, this)
                    .detach();
            }

            /**
             * @brief Setup a client, connects to a given ip and port.

             * @tparam PacketManager: A packetManager class. (Should be derived
             from IPacketManager).
             * @param TCP_port: TCP socket port.
             * @param UDP_port: UDP socket port.
             * @param ip: Server IP address.
             */
            template <class PacketManager>
            void setupClient(
                uint16_t TCP_port, uint16_t UDP_port, const std::string &ip)
            {
                _pacMan = std::make_unique<PacketManager>();
                _isServer = false;
                std::cout << "ENGINE: Connecting to server on port TCP:"
                          << std::to_string(TCP_port)
                          << ", UDP:" << std::to_string(UDP_port)
                          << ", Address: " << ip << std::endl;
                _SocketUDP.initSocket(static_cast<uint16_t>(UDP_port),
                    System::Network::ISocket::CONNECT, ip);
                NetClient cli;
                cli.tcpSocket.initSocket(
                    TCP_port, System::Network::ISocket::CONNECT, ip);
                addClient(cli);
                std::thread(&NetworkingManager::runReadThread, this).detach();
                std::thread(&NetworkingManager::runWriteThread, this).detach();
            }

            /**
             * @brief Set the Client ID.

             * This function should be used by the user for storing an unique
             ID that can identify this client.
             * @param id: An ID
             */
            void setClientID(size_t id);

            /**
             * @brief Get the Client ID

             * @return size_t: The client ID
             */
            size_t getClientID(void) const;

            /**
             * @brief Read all received packets on both UDP and TCP sockets.
             * The PacketManager will be used to extract all complete packets.
             * @return std::vector<std::string>: A vector of packets.
             */
            std::vector<std::string> readAllPackets();

            /**
             * @brief Read all received packets on both UDP and TCP sockets for
             * a specific client.
             * @param cli: The client to read from.
             * @return std::vector<std::string>: A vector of packets.
             */
            std::vector<std::string> readClientPackets(
                NetworkingManager::NetClient &cli);

            /**
             * @brief Send a packet to all connected clients.

             * @param socketType: The type of socket to use: TCP or UDP.
             * @param buffer: The buffer of data to send.
             */
            void sendToAll(System::Network::ISocket::Type socketType,
                const std::string &buffer);

            /**
             * @brief Send a packet to one specific client.

             * @param id: The id of the client.
             * @param socketType: The type of socket to use: TCP or UDP.
             * @param buffer: The buffer of data to send.
             */
            void sendToOne(size_t id,
                System::Network::ISocket::Type socketType,
                const std::string &buffer);

            /**
             * @brief Send a packet all clients except one.

             * @param except_id: The id of the client exception.
             * @param socketType: The type of socket to use: TCP or UDP.
             * @param buffer: The buffer of data to send.
             */
            void sendToOthers(size_t except_id,
                System::Network::ISocket::Type socketType,
                const std::string &buffer);

          protected:
            void engineOnStart(void) override;
            void engineOnTick(float deltaTimeSec) override;
            void engineOnStop(void) override;

            NetClient &addClient(const NetClient &client);
            NetClient &getClient(size_t id);
            void removeClient(size_t id);

            ssize_t identifyClient(const System::Network::ISocket &socket);
            ssize_t identifyClient(
                const std::string &ip, const std::string &port);

            void writeToClient(NetClient &client, const std::string &data,
                System::Network::ISocket::Type socketType);

            void runReadThread();
            void runWriteThread();
            void runConnectThread();

            std::unique_ptr<IPacketManager> _pacMan;
            bool _isServer = false;
            size_t _tcpPort = DEFAULT_TCP_PORT;
            size_t _updPort = DEFAULT_UDP_PORT;
            std::mutex _globalMutex;
            std::condition_variable _writeCondition;
            std::mutex _writeMutex;
            size_t _clientCounter = 0;
            size_t _clientID = 0;
            std::unordered_map<size_t, NetClient> _clients;
            System::Network::TCPSocket _SocketTCP;
            System::Network::UDPSocket _SocketUDP;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_NETWORKING */