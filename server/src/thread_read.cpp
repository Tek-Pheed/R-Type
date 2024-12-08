/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** thread_read
*/

#include <cstddef>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/types.h>
#include "server.hpp"

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "server.hpp"

void server::threadedServerRead()
{
    try {
        System::Network::byteArray arr;
        System::Network::byteArray vect;
        System::Network::socketSetGeneric readfds;
        System::Network::timeoutStruct tv = {{0, 500000}};

        while (true) {
            readfds.clear();
            this->_globalMutex.lock();
            readfds.emplace_back(&_serverSocketUDP);
            for (auto &pair : _clients) {
                Client &ref = pair.second;
                readfds.emplace_back(&ref.tcpSocket);
            }
            this->_globalMutex.unlock();
            System::Network::select(&readfds, nullptr, nullptr, tv);
            if (readfds.size() == 0)
                continue;
            for (auto sock : readfds) {
                vect.clear();
                arr.clear();
                switch (sock->getType()) {
                    case System::Network::ISocket::TCP: {
                        ssize_t id = identifyClient(*sock);
                        if (id == -1)
                            std::cerr << "Unable to idendify client (TCP - read thread)" << std::endl;
                        if (!sock->isOpen()) {
                            std::cout << "[Read Thread] client ("
                                      << std::to_string(id)
                                      << ") closed TCP connection ("
                                      << std::to_string(sock->getUID()) << ")"
                                      << std::endl;
                            removeClient((size_t) id);
                        } else {
                            vect = sock->receive();
                            Client &client = this->getClient((size_t) id);
                            std::string buff =
                                System::Network::decodeString(vect);
                            std::cout
                                << "[Read Thread] Message received on TCP ("
                                << std::to_string(sock->getUID())
                                << ") for client (" << std::to_string(id)
                                << "): " << buff << std::endl;
                            std::unique_lock lock(_globalMutex);
                            client.readBufferTCP += buff;
                            if (client.readBufferTCP.size() > 2
                                && client.readBufferTCP
                                        [client.readBufferTCP.size() - 1]
                                    == '\n'
                                && client.readBufferTCP
                                        [client.readBufferTCP.size() - 2]
                                    == '\t') {
                                this->handle_packet(
                                    (size_t) id, System::Network::ISocket::TCP);
                            }
                        }
                        break;
                    }
                    case System::Network::ISocket::UDP: {
                        // Read UDP

                        std::string addr;
                        uint16_t port;

                        System::Network::UDPSocket *s =
                            static_cast<System::Network::UDPSocket *>(sock);
                        if (s == nullptr)
                            break;
                        vect = s->receiveFrom(addr, port);
                        ssize_t id = identifyClient(addr, std::to_string(port));
                        if (id == -1)
                            std::cerr << "Unable to idendify client (UDP - read thread)" << std::endl;
                        Client &client = getClient((size_t) id); // Garbage created here and returned because unidentified client
                        std::string buff = System::Network::decodeString(vect);
                        std::cout << "[Read Thread] Message received on UDP ("
                                  << std::to_string(sock->getUID())
                                  << ") for client (" << std::to_string(id)
                                  << "): " << buff << std::endl;
                        std::unique_lock lock(_globalMutex);
                        client.readBufferUDP += buff;
                        if (client.readBufferUDP.size() > 2
                            && client.readBufferUDP[client.readBufferUDP.size()
                                   - 1]
                                == '\n'
                            && client.readBufferUDP[client.readBufferUDP.size()
                                   - 2]
                                == '\t') {
                            this->handle_packet(
                                (size_t) id, System::Network::ISocket::UDP);
                        }
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "[Read Thread] failed with exception: " << e.what()
                  << std::endl;
    }
}
