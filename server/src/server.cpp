/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#include "server.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <mutex>
#include <sstream>
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

void identifyClient(std::unordered_map<size_t, Client_t> &clients,
    System::Network::TCPSocket socket);
void identifyClient(std::unordered_map<size_t, Client_t> &clients,
    std::string ip, std::string port);

void server::threadedServerWrite()
{
    try {
        System::Network::byteArray arr;
        System::Network::byteArray vect;
        std::string buffer;
        System::Network::socketSetGeneric writefds;
        System::Network::timeoutStruct tv;
        tv->tv_sec = 0;
        tv->tv_usec = 250000;

        while (true) {
            writefds.clear();
            for (auto &pair : _clients) {
                Client_t &ref = pair.second;
                writefds.emplace_back(&_serverSocketUDP);
                writefds.emplace_back(&ref.tcpSocket);
            }

            System::Network::select(nullptr, &writefds, nullptr, tv);
            if (writefds.size() == 0)
                continue;
        }
    } catch (const std::exception &e) {
        std::cerr << "[Read Thread] failed with exception: " << e.what()
                  << std::endl;
    }
}

void server::threadedServerRead()
{
    try {
        System::Network::byteArray arr;
        System::Network::byteArray vect;
        std::string buffer;
        System::Network::socketSetGeneric readfds;
        System::Network::timeoutStruct tv;
        tv->tv_sec = 0;
        tv->tv_usec = 250000;

        while (true) {
            readfds.clear();
            for (auto &pair : _clients) {
                Client_t &ref = pair.second;
                readfds.emplace_back(&_serverSocketUDP);
                readfds.emplace_back(&ref.tcpSocket);
            }

            System::Network::select(&readfds, nullptr, nullptr, tv);
            if (readfds.size() == 0)
                continue;
            std::cout << "[Read Thread] Event on socket" << std::endl;
            for (auto sock : readfds) {
                vect.clear();
                arr.clear();
                switch (sock->getType()) {
                    case System::Network::ISocket::TCP: {
                        if (!sock->isOpen()) {
                            std::cout << "[Read Thread] TCP Connexion ("
                                      << std::to_string(sock->getUID())
                                      << ") was closed." << std::endl;
                            // identifyClient()
                            // removeClient(id);
                            // return;
                        } else {
                            // Read TCP Socket
                            vect = sock->receive();
                            buffer = System::Network::decodeString(vect);
                            while (buffer[buffer.size() - 1] != '\n'
                                && buffer[buffer.size() - 2] != '\t') {
                                vect = sock->receive();
                                buffer += System::Network::decodeString(vect);
                            }
                            std::cout
                                << "[Read Thread] Message received on TCP ("
                                << std::to_string(sock->getUID())
                                << "): " << buffer << std::endl;
                        }
                        break;
                    }
                    case System::Network::ISocket::UDP: {
                        // Read UDP

                        // identifyClient()
                        System::Network::UDPSocket *s =
                            static_cast<System::Network::UDPSocket *>(sock);
                        if (s == nullptr)
                            break;
                        // s->receiveFrom(std::string &address)

                        // std::cout << "[Read Thread] Message received on UDP
                        // ("
                        //           << std::to_string(sock->getUID())
                        //           << "): " << buffer << std::endl;
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }

        // vect = sock->receive();
        // buffer = System::Network::decodeString(vect);
        // while (buffer[buffer.size() - 1] != '\n'
        //     && buffer[buffer.size() - 2] != '\t') {
        //     vect = sock->receive();
        //     buffer += System::Network::decodeString(vect);
        // }

        // if (dynamic_cast<System::Network::TCPSocket *>(sock)
        //     != nullptr) {
        //     // TCP
        //     if (!sock->isOpen()) {
        //         std::cout << "[Thread " << std::to_string(id)
        //                   << "] TCP Connexion ("
        //                   << std::to_string(sock->getUID())
        //                   << ") was closed, aborting." << std::endl;
        //         removeClient(id);
        //         return;
        //     }
        //     std::cout << "[Thread " << std::to_string(id)
        //               << "] Message received on TCP ("
        //               << std::to_string(sock->getUID())
        //               << "): " << buffer << std::endl;
        // }
        // if (dynamic_cast<System::Network::UDPSocket *>(sock)
        //     != nullptr) {
        //     // UDP
        //     sock->sendData(
        //         System::Network::byteArray({'h', 'l', 'l', 'o'}));
        //     std::cout << "[Thread " << std::to_string(id)
        //               << "] Message received on UDP ("
        //               << std::to_string(sock->getUID())
        //               << "): " << buffer << std::endl;
        // }
        // manage_buffer(buffer);
    } catch (const std::exception &e) {
        std::cerr << "[Read Thread] failed with exception: " << e.what()
                  << std::endl;
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
        Client_t client;
        System::Network::TCPSocket clientSock =
            System::Network::accept(_serverSocketTCP);
        client.tcpSocket = clientSock;
        addClient(client);
    }
}

void server::create_server()
{
    std::cout << "Running server on port TCP:" << std::to_string(TCP_PORT)
              << ", UDP: " << std::to_string(UDP_PORT) << std::endl;
    _serverSocketTCP.initSocket(TCP_PORT, System::Network::ISocket::SERVE);
    _serverSocketUDP.initSocket(UDP_PORT);
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

int server::manage_buffer(std::string buffer)
{
    std::string code = std::string(buffer).substr(0, 3);
    int code_int = is_code_valid(atoi(code.c_str()));
    std::vector<std::string> tokens;

    if (code_int == -1)
        return -1;
    std::string str = buffer.substr(4, buffer.size() - 4);
    std::istringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    switch (code_int) {
        case 0:
            for (size_t i = 0; i < tokens.size(); i++) {
                printf("args: %s\n", tokens[i].c_str());
            }
            handle_player(atoi(code.c_str()), tokens);
            break;
        case 1:
            // handle_enemy(tokens);
            break;
        case 2:
            // handle_terrain(tokens);
            break;
        case 3:
            // handle_mechs(tokens);
            break;

        default: break;
    }
    return 0;
}

int main()
{
    server s;
    System::Network::initNetwork();

    s.create_server();
    std::thread(&server::threadedServerRead, &s).detach();
    std::thread(&server::threadedServerWrite, &s).detach();
    s.handle_connection();

    /// while (true) {
    ///     s.receive_message();
    /// }
    // System::Network::stopNetwork();
}
