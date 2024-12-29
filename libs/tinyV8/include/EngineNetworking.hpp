/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

/**
 * @file Warning: This library is not ready yet. Do not use it yet, it's API
 * will change.
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
        // arguments
        constexpr auto EVENT_ConnectedToServer{"onConnectedToServer"};

        // Event when as a client you lost the connection to the server, no
        // arguments
        constexpr auto EVENT_DisconnectedFromServer{"onDisconnectedToServer"};

        // Event when as a server you get a new connection, no argument
        constexpr auto EVENT_OnServerNewClient{"onServerNewClient"};

        // Event when as a server a client disconnects, ARG: ssize_t (client
        // ID)
        constexpr auto EVENT_OnServerLostClient{"onServerNewClient"};

    }; // namespace Events

    // Implement protocol specific functions
    class IPacketManager {
      public:
        virtual ~IPacketManager() = default;
        virtual std::vector<std::string> splitPackets(
            const System::Network::byteArray &bytes,
            size_t &resultIndexEnd) = 0;
        virtual ssize_t identifyClient(
            const System::Network::byteArray &bytes) = 0;
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

            void setTCPPort(size_t port);
            void setUDPPort(size_t port);
            size_t getTCPPort(void);
            size_t getUDPPort(void);

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
                    TCP_port, System::Network::ISocket::SERVE);
                addClient(cli);
                std::thread(&NetworkingManager::runReadThread, this).detach();
                std::thread(&NetworkingManager::runWriteThread, this).detach();
            }

            void setClientID(size_t id);
            std::vector<std::string> readAllPackets();
            std::vector<std::string>  readClientPackets(NetworkingManager::NetClient &cli);

            void sendToAll(System::Network::ISocket::Type socketType,
                const std::string &buffer);
            void sendToOne(size_t id,
                System::Network::ISocket::Type socketType,
                const std::string &buffer);
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
            size_t _clientCounter;
            size_t _clientID;
            std::unordered_map<size_t, NetClient> _clients;
            System::Network::TCPSocket _SocketTCP;
            System::Network::UDPSocket _SocketUDP;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_NETWORKING */