/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#include "server.hpp"
#include <cstdint>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "protocol.hpp"
#include "server.hpp"

#define TCP_PORT 8081
#define UDP_PORT 8082

server::server()
{
    _clientCounter = 0;
    _serverSocketTCP = System::Network::TCPSocket();
}

server::~server()
{
}

void server::addClient(Client_t &client)
{
    std::unique_lock lock(_mutex);

    _clientCounter++;
    _clients[_clientCounter] = client;
    std::cout << "Created a new client (" << std::to_string(_clientCounter)
              << ") on the server." << std::endl;
}

Client_t &server::getClient(size_t id)
{
    std::unique_lock lock(_mutex);
    return (_clients[id]);
}

void server::removeClient(size_t id)
{
    std::unique_lock lock(_mutex);

    _clients.erase(id);
    std::cout << "Removed a client (" << std::to_string(_clientCounter)
              << ") from the server." << std::endl;
}

void server::threadedClient(size_t id)
{
    Client_t &ref = this->getClient(id);
    std::cout << "[Thread " << std::to_string(id) << "] Created" << std::endl;
    System::Network::UDPSocket udpSocket = System::Network::UDPSocket(
        UDP_PORT + (uint16_t) _clientCounter, "0.0.0.0");
    try {
        std::cout << "[Thread " << std::to_string(id)
                  << "] Created a UDP socket ("
                  << std::to_string(udpSocket.getUID()) << ") on port "
                  << std::to_string(UDP_PORT + _clientCounter) << std::endl;

        System::Network::byteArray arr;
        System::Network::byteArray vect;

        std::string buffer;
        System::Network::socketSetGeneric readfds;

        while (true) {
            readfds.clear();
            readfds.emplace_back(&udpSocket);
            readfds.emplace_back(&ref.tcpSocket);
            System::Network::select(&readfds);
            if (readfds.size() == 0)
                continue;
            std::cout << "[Thread " << std::to_string(id)
                      << "] Event on socket" << std::endl;

            for (auto sock : readfds) {
                vect.clear();
                arr.clear();
                vect = sock->receive();
                buffer = System::Network::decodeString(vect);
                if (buffer[buffer.size() - 1] == '\n'
                    && buffer[buffer.size() - 2] == '\t') {
                    arr.insert(arr.end(), vect.begin(), vect.end());
                    break;
                }
                if (dynamic_cast<System::Network::TCPSocket *>(sock)
                    != nullptr) {
                    // TCP
                    if (!sock->isOpen()) {
                        std::cout << "[Thread " << std::to_string(id)
                                  << "] TCP Connexion ("
                                  << std::to_string(sock->getUID())
                                  << ") was closed, aborting." << std::endl;
                        removeClient(id);
                        return;
                    }
                    std::cout << "[Thread " << std::to_string(id)
                              << "] Message received on TCP ("
                              << std::to_string(sock->getUID())
                              << "): " << buffer << std::endl;
                }
                if (dynamic_cast<System::Network::UDPSocket *>(sock)
                    != nullptr) {
                    // UDP
                    std::cout << "[Thread " << std::to_string(id)
                              << "] Message received on UDP ("
                              << std::to_string(sock->getUID())
                              << "): " << buffer << std::endl;
                }
            }
            // manage_buffer(buffer, static_cast<ssize_t>(buffer.size()));
        }
    } catch (const std::exception &e) {
        std::cerr << "[Thread " << std::to_string(id)
                  << "] failed with exception: " << e.what() << std::endl;
    }
}

void server::handle_connection()
{
    System::Network::socketSetGeneric readfds;

    while (true) {
        readfds.clear();
        readfds.emplace_back(&_serverSocketTCP);
        try {
            System::Network::select(&readfds);
        } catch (const std::exception &s) {
            std::cerr << "An error occured on the server: " << s.what()
                      << std::endl;
        }
        if (readfds.size() == 0)
            continue;
        auto h = System::Network::accept(_serverSocketTCP);
        Client_t c;
        c.server = this;
        c.tcpSocket = h;
        addClient(c);
        std::thread d(&server::threadedClient, this, _clientCounter);
        d.detach();
    }
}

void server::create_server()
{
    std::cout << "Running server on port " << std::to_string(TCP_PORT)
              << std::endl;
    _serverSocketTCP.initSocket(TCP_PORT, System::Network::ISocket::SERVE);
}

int is_code_valid(int code)
{
    if (code >= P_CONN && code <= P_DISCONN)
        return 0;
    if (code >= E_SPAWN && code <= E_DMG)
        return 1;
    if (code >= T_SPAWN && code <= T_DEAD)
        return 2;
    if (code >= M_WAVE && code <= M_GOVER)
        return 3;
    return -1;
}

int main()
{
    server s;
    System::Network::initNetwork();

    s.create_server();

    s.handle_connection();

    /// while (true) {
    ///     s.receive_message();
    /// }
    // System::Network::stopNetwork();
}
