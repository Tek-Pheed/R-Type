/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

#ifndef NET_HPP
#define NET_HPP

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cstddef>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <vector>
#include "protocol.hpp"
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"
#include <condition_variable>
#include <unordered_map>

#define TCP_PORT 8081
#define UDP_PORT 8082

template <typename... Args>
std::string makePacket(int protocolCode, Args... args)
{
    std::string pack = std::to_string(protocolCode);

    for (const auto &ag : {args...}) {
        pack += " " + getString(ag);
    }
    pack += PACKET_END;
    return (pack);
}

class Networking {
    struct NetClient {
      public:
        System::Network::TCPSocket tcpSocket;
        std::string ip;
        uint16_t port;
        std::string readBufferTCP;
        std::string writeBufferTCP;
        std::string readBufferUDP;
        std::string writeBufferUDP;
        bool isReady;
        bool isDisconnected;

        NetClient()
            : tcpSocket(System::Network::TCPSocket()), ip(std::string()),
              port(uint16_t()), readBufferTCP(std::string()),
              writeBufferTCP(std::string()), readBufferUDP(std::string()),
              writeBufferUDP(std::string()), isReady(false),
              isDisconnected(false) {};
    };

  public:
    Networking();
    ~Networking();

    void setupServer();
    void setupClient();

    void setClientID(size_t id);

    std::vector<std::string> readReceivedPackets();
    std::vector<std::string> sendToAll();
    std::vector<std::string> sendToOne(size_t id);
    std::vector<std::string> sendToOthers(size_t except_id);

  protected:
    size_t addNetClient(const NetClient &newNetClient);
    NetClient &getNetClient(size_t id);
    void deleteNetClient(size_t id);

    void runReadThread();
    void runWriteThread();
    void runConnectThread();

    Networking::NetClient &addClient(const NetClient &client);
    Networking::NetClient &getClient(size_t id);
    void removeClient(size_t id);

    ssize_t identifyClient(const System::Network::ISocket &socket);
    ssize_t identifyClient(const std::string &ip, const std::string &port);

    void writeToClient(NetClient &client, const std::string &data,
        System::Network::ISocket::Type socketType);

  private:
    std::mutex _globalMutex;
    std::condition_variable _writeCondition;
    std::mutex _writeMutex;

    size_t _clientCounter;
    size_t _clientID;
    std::unordered_map<size_t, NetClient> _clients;
    System::Network::TCPSocket _serverSocketTCP;
    System::Network::UDPSocket _serverSocketUDP;
};

#endif /* NET_HPP */