// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** server_utils
// */

// #if defined(WIN32)
//     #define NOMINMAX
// #endif

// #include <cstddef>
// #include <iostream>
// #include <mutex>
// #include <string>

// #include "system_network.hpp"
// #include "system_tcp.hpp"

// #include "Networking.hpp"

// ssize_t Networking::identifyClient(const System::Network::ISocket &socket)
// {
//     std::unique_lock lock(_globalMutex);

//     for (auto &pair : _clients) {
//         if (pair.second.tcpSocket.getUID() == socket.getUID())
//             return (ssize_t) pair.first;
//     }
//     return -1;
// }

// ssize_t Networking::identifyClient(
//     const std::string &ip, const std::string &port)
// {
//     std::unique_lock lock(_globalMutex);

//     for (const auto &pair : _clients) {
//         if (pair.second.ip == ip && std::to_string(pair.second.port) == port)
//             return (ssize_t) pair.first;
//     }
//     return -1;
// }

// Networking::NetClient &Networking::addClient(const NetClient &client)
// {
//     std::unique_lock lock(_globalMutex);

//     _clientCounter++;
//     _clients[_clientCounter] = client;
//     std::cout << "Created a new client (" << std::to_string(_clientCounter)
//               << ") on the server." << std::endl;
//     return (_clients[_clientCounter]);
// }

// Networking::NetClient &Networking::getClient(size_t id)
// {
//     std::unique_lock lock(_globalMutex);
//     return (_clients.at(id));
// }

// void Networking::removeClient(size_t id)
// {
//     _globalMutex.lock();
//     _clients.at(id).isDisconnected = true;
//     _globalMutex.unlock();
//     // playerDisconnection(id);
//     std::cout << "Removed a client (" << std::to_string(_clientCounter)
//               << ") from the server." << std::endl;
// }
