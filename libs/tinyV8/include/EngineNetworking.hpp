/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

#ifndef TINY_V8_NETWORKING
#define TINY_V8_NETWORKING

#include "Engine.hpp"

#include <cstddef>
#include <mutex>
#include <string>
#include <vector>
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
                std::vector<std::string> readBufferTCP;
                std::string writeBufferTCP;
                std::vector<std::string> readBufferUDP;
                std::string writeBufferUDP;
                bool isReady;
                bool isDisconnected;

                NetClient()
                    : tcpSocket(System::Network::TCPSocket()),
                      port(uint16_t()),
                      readBufferTCP(std::vector<std::string>()),
                      writeBufferTCP(std::string()),
                      readBufferUDP(std::vector<std::string>()),
                      writeBufferUDP(std::string()), isReady(false),
                      isDisconnected(false) {};
            };

          public:
            explicit NetworkingManager(Core &engineRef);
            ~NetworkingManager();

            void setTCPPort(size_t port);
            void setUDPPort(size_t port);
            size_t getTCPPort(void);
            size_t getUDPPort(void);

            void setupServer(uint16_t TCP_port, uint16_t UDP_port);
            void setupClient(
                uint16_t TCP_port, uint16_t UDP_port, const std::string &ip);
            void setClientID(size_t id);
            std::string readUDPBuffer();
            std::string readTCPBuffer();
            void sendToAll(System::Network::ISocket::Type socketType,
                const std::string &buffer);
            void sendToOne(size_t id,
                System::Network::ISocket::Type socketType,
                const std::string &buffer);
            void sendToOthers(size_t except_id,
                System::Network::ISocket::Type socketType,
                const std::string &buffer);

          protected:
            void onStart(void) override;
            void onTick(float deltaTimeSec) override;
            void onStop(void) override;

            NetClient &addClient(const NetClient &client);
            NetClient &getClient(size_t id);
            void removeClient(size_t id);

            ssize_t identifyClient(const System::Network::ISocket &socket);
            ssize_t identifyClient(
                const std::string &ip, const std::string &port);

            void writeToClient(NetClient &client, const std::string &data,
                System::Network::ISocket::Type socketType);
            ssize_t authenticateUDPClient(
                const System::Network::byteArray &packet);

            void runReadThread();
            void runWriteThread();
            void runConnectThread();

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