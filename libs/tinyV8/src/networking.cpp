/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

#include <sstream>
#include <iostream>
#include <mutex>
#include <string>
#include "EngineNetworking.hpp"

using namespace Engine::Feature;

NetworkingManager::NetworkingManager(Core &engineRef)
    : AEngineFeature(engineRef)
{
}

NetworkingManager::~NetworkingManager()
{
}

void NetworkingManager::onStart(void)
{
    return;
}

void NetworkingManager::onTick(float deltaTimeSec)
{
    (void) deltaTimeSec;
    return;
}

void NetworkingManager::onStop(void)
{
    return;
}

void NetworkingManager::setTCPPort(size_t port)
{
    _tcpPort = port;
}

void NetworkingManager::setUDPPort(size_t port)
{
    _updPort = port;
}

size_t NetworkingManager::getTCPPort(void)
{
    return (_tcpPort);
}

size_t NetworkingManager::getUDPPort(void)
{
    return (_updPort);
}

void NetworkingManager::setupServer(uint16_t TCP_port, uint16_t UDP_port)
{
    _isServer = true;
    _SocketTCP.initSocket(TCP_port, System::Network::ISocket::SERVE);
    _SocketUDP.initSocket(UDP_port);
    std::cout << "Running server on port TCP:" << std::to_string(TCP_port)
              << ", UDP:" << std::to_string(UDP_port) << std::endl;

    std::thread(&NetworkingManager::runReadThread, this).detach();
    std::thread(&NetworkingManager::runWriteThread, this).detach();
    std::thread(&NetworkingManager::runConnectThread, this).detach();
}

void NetworkingManager::setClientID(size_t id)
{
    _clientID = id;
}

void NetworkingManager::setupClient(
    uint16_t TCP_port, uint16_t UDP_port, const std::string &ip)
{
    _isServer = false;
    std::cout << "Connecting to server on port TCP:"
              << std::to_string(TCP_port)
              << ", UDP:" << std::to_string(UDP_port) << ", Address: " << ip
              << std::endl;
    _SocketUDP.initSocket(static_cast<uint16_t>(UDP_port),
        System::Network::ISocket::CONNECT, ip);
    NetClient cli;
    cli.tcpSocket.initSocket(TCP_port, System::Network::ISocket::SERVE);
    addClient(cli);
    std::thread(&NetworkingManager::runReadThread, this).detach();
    std::thread(&NetworkingManager::runWriteThread, this).detach();
}

void NetworkingManager::sendToAll(
    System::Network::ISocket::Type socketType, const std::string &buffer)
{
    for (auto &client : _clients) {
        writeToClient(client.second, buffer, socketType);
    }
}

void NetworkingManager::sendToOthers(size_t except_id,
    System::Network::ISocket::Type socketType, const std::string &buffer)
{
    for (auto &client : _clients) {
        if (client.first != except_id)
            writeToClient(client.second, buffer, socketType);
    }
}

void NetworkingManager::sendToOne(size_t id,
    System::Network::ISocket::Type socketType, const std::string &buffer)
{
    for (auto &client : _clients) {
        if (client.first == id)
            writeToClient(client.second, buffer, socketType);
    }
}

NetworkingManager::NetClient &NetworkingManager::addClient(
    const NetClient &client)
{
    std::unique_lock lock(_globalMutex);

    _clientCounter++;
    _clients[_clientCounter] = client;
    std::cout << "Created a new client (" << std::to_string(_clientCounter)
              << ") on the server." << std::endl;
    return (_clients[_clientCounter]);
}

NetworkingManager::NetClient &NetworkingManager::getClient(size_t id)
{
    std::unique_lock lock(_globalMutex);

    return (_clients.at(id));
}

void NetworkingManager::removeClient(size_t id)
{
    std::unique_lock lock(_globalMutex);

    _clients.at(id).isDisconnected = true;
    std::cout << "Removed a client (" << std::to_string(_clientCounter)
              << ") from the server." << std::endl;
}

ssize_t NetworkingManager::identifyClient(
    const System::Network::ISocket &socket)
{
    std::unique_lock lock(_globalMutex);

    for (auto &pair : _clients) {
        if (pair.second.tcpSocket.getUID() == socket.getUID())
            return (ssize_t) pair.first;
    }
    return -1;
}

ssize_t NetworkingManager::identifyClient(
    const std::string &ip, const std::string &port)
{
    std::unique_lock lock(_globalMutex);

    for (const auto &pair : _clients) {
        if (pair.second.ip == ip && std::to_string(pair.second.port) == port)
            return (ssize_t) pair.first;
    }
    return -1;
}


// This function should be handeled by the game
ssize_t NetworkingManager::authenticateUDPClient(
    const System::Network::byteArray &packet)
{
    std::string decoded = System::Network::decodeString(packet);
    if (decoded.starts_with(std::to_string(902))) {
        std::string::size_type p = decoded.find(' ');
        auto id = std::strtol(&decoded[p], nullptr, 10);
        if (id == LLONG_MAX || id == LLONG_MIN)
            return (-1);
        return (id);
    }
    return (-1);
}

void serializeString(const std::string &str, std::ostream &out)
{
    size_t size = str.size();
    out.write(reinterpret_cast<const char *>(&size), sizeof(size));
    out.write(str.data(), static_cast<std::streamsize>(size));
}

void NetworkingManager::writeToClient(NetworkingManager::NetClient &client,
    const std::string &data, System::Network::ISocket::Type socketType)
{
    std::ostringstream out;
    serializeString(data, out);
    std::string serializedData = out.str();
    std::unique_lock lock(_writeMutex);

    if (socketType == System::Network::ISocket::TCP) {
        client.writeBufferTCP += serializedData;
    }
    if (socketType == System::Network::ISocket::UDP) {
        client.writeBufferUDP += serializedData;
    }
    _writeCondition.notify_one();
}
