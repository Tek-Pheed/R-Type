/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <sys/types.h>
#include "EngineNetworking.hpp"
#include "system_network.hpp"

using namespace Engine::Feature;

NetworkingManager::NetworkingManager(Core &engineRef)
    : AEngineFeature(engineRef)
{
    System::Network::initNetwork();
}

NetworkingManager::~NetworkingManager()
{
}

void NetworkingManager::engineOnStart(void)
{
    return;
}

void NetworkingManager::engineOnTick(float deltaTimeSec)
{
    (void) deltaTimeSec;
    return;
}

void NetworkingManager::engineOnStop(void)
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

size_t NetworkingManager::getTCPPort(void) const
{
    return (_tcpPort);
}

size_t NetworkingManager::getUDPPort(void) const
{
    return (_updPort);
}

void NetworkingManager::setClientID(size_t id)
{
    _clientID = id;
}

size_t NetworkingManager::getClientID(void) const
{
    return (_clientID);
}

std::vector<std::string> NetworkingManager::readClientPackets(
    NetworkingManager::NetClient &cli)
{
    std::unique_lock lock(_globalMutex);
    std::vector<std::string> packets;
    size_t endIndex = 0;
    System::Network::byteArray bytes;

    bytes = cli.readBufferTCP;
    packets = _pacMan->splitPackets(bytes, endIndex);
    cli.readBufferTCP.erase(cli.readBufferTCP.begin(),
        cli.readBufferTCP.begin() + (ssize_t) endIndex);
    endIndex = 0;
    bytes = cli.readBufferUDP;
    auto udpPackets = _pacMan->splitPackets(bytes, endIndex);
    packets.insert(packets.end(), udpPackets.begin(), udpPackets.end());
    return (packets);
}

std::vector<std::string> NetworkingManager::readAllPackets()
{
    std::vector<std::string> packets;

    if (_isServer) {
        for (auto &client : _clients) {
            auto packs = readClientPackets(client.second);
            packets.insert(packets.end(), packs.begin(), packs.end());
        }
    } else {
        NetworkingManager::NetClient &cli = getClient(1);
        auto packs = readClientPackets(cli);
        packets.insert(packets.end(), packs.begin(), packs.end());
    }
    return (packets);
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
    std::cout << "ENGINE: Created a new client ("
              << std::to_string(_clientCounter) << ") on the server."
              << std::endl;
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
    std::cout << "ENGINE: Removed a client (" << std::to_string(_clientCounter)
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

void NetworkingManager::writeToClient(NetworkingManager::NetClient &client,
    const std::string &data, System::Network::ISocket::Type socketType)
{
    std::ostringstream out;
    _pacMan->serializeString(data, out);
    std::string serializedData = out.str();
    std::unique_lock lock(_writeMutex);
    auto encoded = System::Network::encodeString(serializedData);

    if (socketType == System::Network::ISocket::TCP) {
        client.writeBufferTCP.insert(
            client.writeBufferTCP.end(), encoded.begin(), encoded.end());
    }
    if (socketType == System::Network::ISocket::UDP) {
        client.writeBufferUDP.insert(
            client.writeBufferUDP.begin(), encoded.begin(), encoded.end());
    }
    _writeCondition.notify_one();
}
