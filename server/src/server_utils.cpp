/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server_utils
*/

#include <iostream>

#include "server.hpp"

ssize_t server::identifyClient(const System::Network::ISocket &socket)
{
    std::unique_lock lock(_globalMutex);

    for (auto &pair : _clients) {
        if (pair.second.tcpSocket.getUID() == socket.getUID())
            return (ssize_t) pair.first;
    }
    return -1;
}

ssize_t server::identifyClient(const std::string &ip, const std::string &port)
{
    std::unique_lock lock(_globalMutex);

    for (const auto &pair : _clients) {
        if (pair.second.ip == ip && std::to_string(pair.second.port) == port)
            return (ssize_t) pair.first;
    }
    return -1;
}

Client &server::addClient(const Client &client)
{
    std::unique_lock lock(_globalMutex);

    _clientCounter++;
    _clients[_clientCounter] = client;
    std::cout << "Created a new client (" << std::to_string(_clientCounter)
              << ") on the server." << std::endl;
    return (_clients[_clientCounter]);
}

Client &server::getClient(size_t id)
{
    std::unique_lock lock(_globalMutex);
    return (_clients.at(id));
}

void server::removeClient(size_t id)
{
    std::unique_lock lock(_globalMutex);

    _clients.erase(id);
    std::cout << "Removed a client (" << std::to_string(_clientCounter)
              << ") from the server." << std::endl;
}
