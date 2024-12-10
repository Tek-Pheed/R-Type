/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** thread_read
*/

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/types.h>
#include "protocol.hpp"
#include "server.hpp"

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "server.hpp"

// This will need to be more robust
ssize_t server::authenticateUDPClient(const System::Network::byteArray &packet)
{
    std::string decoded = System::Network::decodeString(packet);
    if (decoded.starts_with(std::to_string(C_START_UDP))) {
        std::string::size_type p = decoded.find(' ');
        auto id = std::strtol(&decoded[p], nullptr, 10);
        if (id == LLONG_MAX || id == LLONG_MIN)
            return (-1);
        return (id);
    }
    return (-1);
}

void server::threadedServerRead()
{
    try {
        System::Network::byteArray arr;
        System::Network::byteArray vect;
        System::Network::socketSetGeneric readfds;
        System::Network::timeoutStruct tv = {{0, 50000}};

        while (true) {
            readfds.clear();
            this->_globalMutex.lock();
            readfds.emplace_back(&_serverSocketUDP);
            for (auto &pair : _clients) {
                Client &ref = pair.second;
                if (!ref.isDisconnected)
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
                            std::cerr << "Unable to idendify client (TCP - "
                                         "read thread)"
                                      << std::endl;
                        if (!sock->isOpen()) {
                            std::cout << "[Read Thread] client ("
                                      << std::to_string(id)
                                      << ") closed TCP connection ("
                                      << std::to_string(sock->getUID()) << ")"
                                      << std::endl;
                            removeClient((size_t) id);
                        } else {
                            Client &client = this->getClient((size_t) id);
                            if (client.isDisconnected)
                                continue;
                            vect = sock->receive();
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
                                this->handle_packet((size_t) id,
                                    System::Network::ISocket::TCP);
                            }
                        }
                        break;
                    }
                    case System::Network::ISocket::UDP: {
                        std::string addr;
                        uint16_t port;
                        System::Network::UDPSocket *s =
                            static_cast<System::Network::UDPSocket *>(sock);
                        if (s == nullptr)
                            break;
                        vect = s->receiveFrom(addr, port);
                        ssize_t id =
                            identifyClient(addr, std::to_string(port));
                        if (id == -1) {
                            id = authenticateUDPClient(vect);
                            if (id == -1) {
                                std::cerr
                                    << "[Read Thread] Failed to identify "
                                       "client on UDP ("
                                    << std::to_string(sock->getUID())
                                    << ") with packet: "
                                    << System::Network::decodeString(vect)
                                    << std::endl;
                                continue;
                            } else {
                                std::cout << "[Read Thread] Successfully "
                                             "authenticated client ("
                                          << std::to_string(id)
                                          << ") on UDP Socket ("
                                          << std::to_string(sock->getUID())
                                          << ")" << std::endl;
                                Client &client = getClient((size_t) id);
                                if (client.isDisconnected)
                                    continue;
                                client.ip = addr;
                                client.port = port;
                                this->handle_packet((size_t) id,
                                    System::Network::ISocket::UDP);
                            }
                        }
                        Client &client = getClient((size_t) id);
                        if (client.isDisconnected)
                            continue;
                        std::string buff = System::Network::decodeString(vect);
                        std::cout << "[Read Thread] Message received on UDP ("
                                  << std::to_string(sock->getUID())
                                  << ") for client (" << std::to_string(id)
                                  << "): " << buff << std::endl;
                        _globalMutex.lock();
                        client.readBufferUDP += buff;
                        _globalMutex.unlock();
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
