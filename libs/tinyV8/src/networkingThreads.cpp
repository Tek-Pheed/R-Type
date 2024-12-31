/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** networkingThreads
*/

#include <iostream>
#include <mutex>
#include <string>
#include <sys/types.h>
#include "EngineNetworking.hpp"
#include "system_network.hpp"
#include "system_udp.hpp"

using namespace Engine::Feature;

void NetworkingManager::runConnectThread()
{
    System::Network::socketSetGeneric readfds;

    while (true) {
        readfds.clear();
        readfds.emplace_back(&_SocketTCP);
        try {
            System::Network::select(&readfds);
        } catch (const std::exception &s) {
            std::cout << "ENGINE: An error occured in the server: " << s.what()
                      << std::endl;
        }
        if (readfds.size() == 0)
            continue;
        NetClient cl;
        cl.tcpSocket = System::Network::accept(_SocketTCP);
        addClient(cl);
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
            bool shouldMonitor = false;
            for (auto &pair : _clients) {
                NetClient &ref = pair.second;
                if (ref.writeBufferUDP.size() > 0 && !ref.isDisconnected)
                    shouldMonitor = true;
                if ((ref.writeBufferTCP.size() > 0) && !ref.isDisconnected)
                    writefds.emplace_back(&ref.tcpSocket);
            }
            if (shouldMonitor)
                writefds.emplace_back(&_SocketUDP);
            System::Network::select(nullptr, &writefds, nullptr, tv);
            shouldWait = true;
            if (writefds.size() == 0)
                continue;
            std::unique_lock lc(_writeFinished);
            for (auto sock : writefds) {
                ssize_t len = 0;
                int removeID = -1;
                bool can_remove = false;
                switch (sock->getType()) {
                    case System::Network::ISocket::TCP: {
                        ssize_t id = this->identifyClient(*sock);
                        if (id == -1)
                            std::cout << "ENGINE: [Write Thread] Unable to "
                                         "idendify client"
                                      << std::endl;
                        NetClient &client = this->getClient((size_t) id);
                        if (client.isDisconnected) {
                            removeID = (int) id;
                            if (client.writeBufferTCP.size() == 0
                                && client.writeBufferUDP.size() == 0)
                                can_remove = true;
                        }
                        if (!sock->isOpen()) {
                            std::cout << "ENGINE: [Write Thread] client ("
                                      << std::to_string(id)
                                      << ") closed TCP connection ("
                                      << std::to_string(sock->getUID()) << ")"
                                      << std::endl;
                            this->removeClient((size_t) id);
                        }
                        if (client.writeBufferTCP.size() > 0) {
                            std::unique_lock lock(_writeMutex);
                            len = sock->sendData(client.writeBufferTCP);
                            std::cout << "ENGINE: [Write Thread] Sending: "
                                      << System::Network::decodeString(
                                             client.writeBufferTCP)
                                             .substr(0, (size_t) len)
                                      << "to client (" << std::to_string(id)
                                      << ") on TCP connection ("
                                      << std::to_string(sock->getUID())
                                      << ") [" << len << "/"
                                      << client.writeBufferTCP.size()
                                      << " written]" << std::endl;
                            if ((size_t) len < client.writeBufferTCP.size())
                                shouldWait = false;
                            client.writeBufferTCP.erase(
                                client.writeBufferTCP.begin(),
                                client.writeBufferTCP.begin() + len);
                        }
                        break;
                    }
                    case System::Network::ISocket::UDP: {
                        if (sock->getUID() == this->_SocketUDP.getUID()) {
                            for (auto &cl : _clients) {
                                size_t index = cl.first;
                                auto &cli = cl.second;
                                if (cli.isDisconnected) {
                                    removeID = (int) index;
                                    if (cli.writeBufferTCP.size() == 0
                                        && cli.writeBufferUDP.size() == 0)
                                        can_remove = true;
                                }
                                if (_isServer) {
                                    if (cli.port == 0 || cli.ip.empty()
                                        || cli.writeBufferUDP.size() == 0)
                                        continue;
                                    std::unique_lock lock(_writeMutex);
                                    len = _SocketUDP.sendDataTo(
                                        cli.writeBufferUDP, cli.ip, cli.port);
                                } else {
                                    std::unique_lock lock(_writeMutex);
                                    len = _SocketUDP.sendData(
                                        cli.writeBufferUDP);
                                }
                                std::unique_lock lock(_writeMutex);
                                std::cout
                                    << "ENGINE: [Write Thread] Sending: "
                                    << System::Network::decodeString(
                                           cli.writeBufferUDP)
                                           .substr(0, (size_t) len)
                                    << "to client (" << std::to_string(index)
                                    << ") on UDP connection ("
                                    << std::to_string(sock->getUID()) << ") ["
                                    << len << "/" << cli.writeBufferUDP.size()
                                    << " bytes]" << std::endl;
                                if ((size_t) len < cli.writeBufferUDP.size())
                                    shouldWait = false;
                                std::cout << "Before:"
                                          << System::Network::decodeString(
                                                 cli.writeBufferUDP)
                                          << std::endl;
                                cli.writeBufferUDP.erase(
                                    cli.writeBufferUDP.begin(),
                                    cli.writeBufferUDP.begin() + len);
                                std::cout << "After: "
                                          << System::Network::decodeString(
                                                 cli.writeBufferUDP)
                                          << std::endl;
                            }
                        }
                    }
                    default: {
                        break;
                    };
                }
                if (removeID != -1 && can_remove == true) {
                    if (_isServer) {
                        AEngineFeature::_engineRef.triggerEvent(
                            Events::EVENT_OnServerLostClient, removeID);
                    } else {
                        AEngineFeature::_engineRef.triggerEvent(
                            Events::EVENT_DisconnectedFromServer);
                    }
                    std::unique_lock lock(_globalMutex);
                    _clients.erase((size_t) removeID);
                }
                _writeFinishedCond.notify_all();
            }
        } catch (const std::exception &e) {
            std::cout << "ENGINE: [Write Thread] failed with exception: "
                      << e.what() << ", ignoring..." << std::endl;
        }
    }
}

void NetworkingManager::runReadThread()
{
    System::Network::byteArray arr;
    System::Network::byteArray vect;
    System::Network::socketSetGeneric readfds;
    System::Network::timeoutStruct tv = {{0, 500000}};

    while (true) {
        try {
            readfds.clear();
            this->_globalMutex.lock();
            readfds.emplace_back(&_SocketUDP);
            for (auto &pair : _clients) {
                NetClient &ref = pair.second;
                if (!ref.isDisconnected)
                    readfds.emplace_back(&ref.tcpSocket);
            }
            this->_globalMutex.unlock();
            System::Network::select(&readfds, nullptr, nullptr, tv);
            if (readfds.size() == 0)
                continue;
            for (auto sock : readfds) {
                switch (sock->getType()) {
                    case System::Network::ISocket::TCP: {
                        vect.clear();
                        arr.clear();
                        ssize_t id = 1;
                        if (_isServer) {
                            id = identifyClient(*sock);
                            if (id == -1)
                                std::cout << "ENGINE: [Read Thread] Unable to "
                                             "idendify client"
                                          << std::endl;
                        }
                        if (!sock->isOpen()) {
                            std::cout << "ENGINE: [Read Thread] client ("
                                      << std::to_string(id)
                                      << ") closed TCP connection ("
                                      << std::to_string(sock->getUID()) << ")"
                                      << std::endl;
                            if (_isServer) {
                                AEngineFeature::_engineRef.triggerEvent(
                                    Events::EVENT_OnServerLostClient, id);
                                removeClient((size_t) id);
                            } else {
                                AEngineFeature::_engineRef.triggerEvent(
                                    Events::EVENT_DisconnectedFromServer);
                                getClient(1).isDisconnected = true;
                            }
                        } else {
                            NetClient &client = this->getClient((size_t) id);
                            if (client.isDisconnected)
                                continue;
                            vect = sock->receive();
                            std::cout
                                << "ENGINE: [Read Thread] Message received "
                                   "on TCP ("
                                << std::to_string(sock->getUID())
                                << ") for client (" << std::to_string(id)
                                << "): " << System::Network::decodeString(vect)
                                << std::endl;
                            _globalMutex.lock();
                            client.readBufferTCP.insert(
                                client.readBufferTCP.end(), vect.begin(),
                                vect.end());
                            _globalMutex.unlock();
                            AEngineFeature::_engineRef.triggerEvent(
                                Events::EVENT_OnDataReceived);
                        }
                        break;
                    }
                    case System::Network::ISocket::UDP: {
                        vect.clear();
                        arr.clear();
                        std::string addr;
                        uint16_t port;
                        System::Network::UDPSocket *s =
                            static_cast<System::Network::UDPSocket *>(sock);
                        if (s == nullptr)
                            break;
                        vect = s->receiveFrom(addr, port);
                        ssize_t id = 1;
                        if (_isServer)
                            id = identifyClient(addr, std::to_string(port));
                        if (id == -1) {
                            id = _pacMan->identifyClient(vect);
                            if (id == -1) {
                                std::cout
                                    << "ENGINE: [Read Thread] Failed to "
                                       "identify "
                                       "client on UDP ("
                                    << std::to_string(sock->getUID())
                                    << ") with packet: "
                                    << System::Network::decodeString(vect)
                                    << std::endl;
                                continue;
                            } else {
                                std::cout
                                    << "ENGINE: [Read Thread] Successfully "
                                       "authenticated client ("
                                    << std::to_string(id)
                                    << ") on UDP Socket ("
                                    << std::to_string(sock->getUID()) << ")"
                                    << std::endl;
                                NetClient &client = getClient((size_t) id);
                                if (client.isDisconnected)
                                    continue;
                                client.ip = addr;
                                client.port = port;
                            }
                        } else {
                            getClient((size_t) id).ip = addr;
                            getClient((size_t) id).port = port;
                        }
                        NetClient &client = getClient((size_t) id);
                        if (client.isDisconnected)
                            continue;
                        std::cout
                            << "ENGINE: [Read Thread] Message received "
                               "on UDP ("
                            << std::to_string(sock->getUID())
                            << ") for client (" << std::to_string(id)
                            << "): " << System::Network::decodeString(vect)
                            << std::endl;
                        _globalMutex.lock();
                        if (!(client.readBufferUDP.size()
                                >= UDP_PACKET_MAX_SIZE
                                    * UDP_BUFFER_MAX_QUEUED_PACKETS)) {
                            client.readBufferUDP.insert(
                                client.readBufferUDP.end(), vect.begin(),
                                vect.end());
                        } else {
                            std::cout << "ENGINE: [Read Thread] The packet "
                                         "queue is full on UDP ("
                                      << std::to_string(sock->getUID())
                                      << ") for client (" << std::to_string(id)
                                      << "): Ignoring extra packets."
                                      << std::endl;
                        }
                        _globalMutex.unlock();
                        AEngineFeature::_engineRef.triggerEvent(
                            Events::EVENT_OnDataReceived);
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        } catch (const std::exception &e) {
            std::cout << "ENGINE: [Read Thread] failed with exception: "
                      << e.what() << ", ignoring..." << std::endl;
        }
    }
}
