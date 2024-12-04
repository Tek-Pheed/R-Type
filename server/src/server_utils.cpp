/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server_utils
*/

#include <iostream>

#include "server.hpp"

size_t server::identifyClient(System::Network::TCPSocket socket)
{
    for (auto &pair : _clients) {
        if (pair.second.tcpSocket.getUID() == socket.getUID())
            return pair.first;
    }
    return 0;
}

size_t server::identifyClient(std::string ip, std::string port)
{
    for (auto &pair : _clients) {
        if (pair.second.ip == ip && std::to_string(pair.second.port) == port)
            return pair.first;
    }
    return 0;
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
