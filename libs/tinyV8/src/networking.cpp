/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Networking
*/

#include <iostream>
#include <string>
#include "EngineNetworking.hpp"

using namespace Engine::Feature;

NetworkingManager::NetworkingManager(Core &engineRef) : AEngineFeature(engineRef)
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

NetworkingManager::NetClient &NetworkingManager::addClient(const NetClient &client)
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
    _globalMutex.lock();
    _clients.at(id).isDisconnected = true;
    _globalMutex.unlock();
    std::cout << "Removed a client (" << std::to_string(_clientCounter)
              << ") from the server." << std::endl;
}

void NetworkingManager::runConnectThread()
{
    System::Network::socketSetGeneric readfds;

    while (true) {
        readfds.clear();
        readfds.emplace_back(&_SocketTCP);
        try {
            System::Network::select(&readfds);
        } catch (const std::exception &s) {
            std::cout << "An error occured on the server: " << s.what()
                      << std::endl;
        }
        if (readfds.size() == 0)
            continue;
        NetClient cl;
        cl.tcpSocket = System::Network::accept(_SocketTCP);
        addClient(cl);
        // std::string toWrite = makePacket(C_INIT_UDP, _clientCounter);
        // writeToClient(client, toWrite, System::Network::ISocket::TCP);
    }
}

void NetworkingManager::runWriteThread()
{
    System::Network::socketSetGeneric writefds;
    System::Network::timeoutStruct tv = {{0, 50000}};
    bool shouldWait = true;

    while (true) {
        try {
            if (shouldWait) {
                std::unique_lock lock(_writeMutex);
                _writeCondition.wait(lock);
            }
            writefds.clear();
            writefds.emplace_back(&_SocketUDP);
            for (auto &pair : _clients) {
                NetClient &ref = pair.second;
                if ((ref.writeBufferTCP.length() > 0
                        || ref.writeBufferUDP.length() > 0)
                    && !ref.isDisconnected)
                    writefds.emplace_back(&ref.tcpSocket);
            }
            System::Network::select(nullptr, &writefds, nullptr, tv);
            shouldWait = true;
            if (writefds.size() == 0)
                continue;
            for (auto sock : writefds) {
                ssize_t len = 0;
                int removeID = -1;
                bool can_remove = false;
                switch (sock->getType()) {
                    case System::Network::ISocket::TCP: {
                        ssize_t id = this->identifyClient(*sock);
                        if (id == -1)
                            std::cout << "Unable to idendify client (TCP - "
                                         "write thread)"
                                      << std::endl;
                        NetClient &client = this->getClient((size_t) id);
                        if (client.isDisconnected) {
                            removeID = (int) id;
                            if (client.writeBufferTCP.length() == 0
                                && client.writeBufferUDP.length() == 0)
                                can_remove = true;
                        }
                        if (!sock->isOpen()) {
                            std::cout << "[Write Thread] client ("
                                      << std::to_string(id)
                                      << ") closed TCP connection ("
                                      << std::to_string(sock->getUID()) << ")"
                                      << std::endl;
                            this->removeClient((size_t) id);
                        }
                        if (client.writeBufferTCP.size() > 0) {
                            len = sock->sendData(System::Network::encodeString(
                                client.writeBufferTCP));
                            std::cout << "[Write Thread] Sending: "
                                      << client.writeBufferTCP.substr(
                                             0, (size_t) len)
                                      << "to client (" << std::to_string(id)
                                      << ") on TCP connection ("
                                      << std::to_string(sock->getUID()) << ")"
                                      << std::endl;
                            if ((size_t) len < client.writeBufferTCP.length())
                                shouldWait = false;
                            client.writeBufferTCP.erase(0, (size_t) len);
                        }
                        break;
                    }
                    case System::Network::ISocket::UDP: {
                        if (sock->getUID()
                            == this->_SocketUDP.getUID()) {
                            for (auto &cl : _clients) {
                                size_t index = cl.first;
                                auto &cli = cl.second;
                                if (cli.isDisconnected) {
                                    removeID = (int) index;
                                    if (cli.writeBufferTCP.length() == 0
                                        && cli.writeBufferUDP.length() == 0)
                                        can_remove = true;
                                }
                                if (cli.port == 0 || cli.ip.empty()
                                    || cli.writeBufferUDP.length() == 0)
                                    continue;
                                len = _SocketUDP.sendDataTo(
                                    System::Network::encodeString(
                                        cli.writeBufferUDP),
                                    cli.ip, cli.port);
                                std::cout
                                    << "[Write Thread] Sending: "
                                    << cli.writeBufferUDP.substr(
                                           0, (size_t) len)
                                    << "to client (" << std::to_string(index)
                                    << ") on UDP connection ("
                                    << std::to_string(sock->getUID()) << ")"
                                    << std::endl;
                                if ((size_t) len < cli.writeBufferUDP.length())
                                    shouldWait = false;
                                cli.writeBufferUDP.erase(0, (size_t) len);
                            }
                        }
                    }
                    default: {
                        break;
                    };
                }
                if (removeID != -1 && can_remove == true) {
                    std::unique_lock lock(_globalMutex);
                    _clients.erase((size_t) removeID);
                }
            }
        } catch (const std::exception &e) {
            std::cout << "[Write Thread] failed with exception: " << e.what()
                      << std::endl;
        }
    }
}

