/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** thread_write
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <iostream>
#include <string>

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "Networking.hpp"

void Networking::writeToClient(Networking::NetClient &client, const std::string &data,
    System::Network::ISocket::Type socketType)
{
    std::unique_lock lock(_writeMutex);

    if (socketType == System::Network::ISocket::TCP) {
        client.writeBufferTCP += data;
    }
    if (socketType == System::Network::ISocket::UDP) {
        client.writeBufferUDP += data;
    }
    _writeCondition.notify_one();
}

void Networking::setClientID(size_t id) {
    _clientID = id;
}

void Networking::setupServer()
{
    _serverSocketTCP.initSocket(TCP_PORT, System::Network::ISocket::SERVE);
    _serverSocketUDP.initSocket(UDP_PORT);
    std::cout << "Running server on port TCP:" << std::to_string(TCP_PORT)
              << ", UDP:" << std::to_string(UDP_PORT) << std::endl;

    std::thread(&Networking::runReadThread, this).detach();
    std::thread(&Networking::runWriteThread, this).detach();
    std::thread(&Networking::runConnectThread, this).detach();
}
