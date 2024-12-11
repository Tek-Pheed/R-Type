/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** thread_write
*/

#include <cstddef>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include "server.hpp"

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "server.hpp"

void server::threadedServerWrite()
{
    try {
        System::Network::socketSetGeneric writefds;
        System::Network::timeoutStruct tv = {{0, 50000}};
        bool shouldWait = true;

        while (true) {
            if (shouldWait) {
                std::unique_lock lock(_writeMutex);
                _writeCondition.wait(lock);
            }
            writefds.clear();
            writefds.emplace_back(&_serverSocketUDP);
            for (auto &pair : _clients) {
                Client &ref = pair.second;
                if ((ref.writeBufferTCP.length() > 0
                    || ref.writeBufferUDP.length() > 0 )&& !ref.isDisconnected)
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
                        Client &client = this->getClient((size_t) id);
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
                            == this->_serverSocketUDP.getUID()) {
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
                                len = _serverSocketUDP.sendDataTo(
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
        }
    } catch (const std::exception &e) {
        std::cout << "[Write Thread] failed with exception: " << e.what()
                  << std::endl;
    }
}
