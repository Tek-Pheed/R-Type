// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** thread_write
// */

// #if defined(WIN32)
//     #define NOMINMAX
// #endif

// #include <climits>
// #include <iostream>
// #include <sstream>
// #include <string>
// #include <thread>

// #include "system_network.hpp"
// #include "system_tcp.hpp"
// #include "system_udp.hpp"

// #include "Networking.hpp"

// Networking::Networking()
// {
// }

// Networking::~Networking()
// {
// }

// void Networking::setClientID(size_t id)
// {
//     _clientID = id;
// }

// std::string deserializeString(std::istream &in)
// {
//     size_t size;
//     in.read(reinterpret_cast<char *>(&size), sizeof(size));
//     std::vector<char> buffer(size);
//     in.read(buffer.data(), static_cast<std::streamsize>(size));
//     return std::string(buffer.begin(), buffer.end());
// }

// std::vector<std::string> Networking::readReceivedPackets()
// {
//     std::unique_lock lock(_globalMutex);
//     std::vector<std::string> packets;

//     for (const auto &cli : _clients) {
//         for (const auto &buff : cli.second.readBufferTCP) {
//             std::istringstream in(std::string(buff, buff.size()));
//             std::string deserializedBuff = deserializeString(in);
//             packets.push_back(deserializedBuff);
//         }
//         for (const auto &buff : cli.second.readBufferUDP) {
//             std::istringstream in(std::string(buff, buff.size()));
//             std::string deserializedBuff = deserializeString(in);
//             packets.push_back(buff);
//         }
//     }
//     return (packets);
// }

// void Networking::setupServer(uint16_t TCP_port, uint16_t UDP_port)
// {
//     _isServer = true;
//     _SocketTCP.initSocket(TCP_port, System::Network::ISocket::SERVE);
//     _SocketUDP.initSocket(UDP_port);
//     std::cout << "Running server on port TCP:" << std::to_string(TCP_port)
//               << ", UDP:" << std::to_string(UDP_port) << std::endl;

//     std::thread(&Networking::runReadThread, this).detach();
//     std::thread(&Networking::runWriteThread, this).detach();
//     std::thread(&Networking::runConnectThread, this).detach();
// }

// void Networking::setupClient(
//     uint16_t TCP_port, uint16_t UDP_port, const std::string &ip)
// {
//     _isServer = false;
//     std::cout << "Connecting to server on port TCP:"
//               << std::to_string(TCP_PORT)
//               << ", UDP:" << std::to_string(UDP_PORT) << ", Address: " << ip
//               << std::endl;
//     _SocketUDP.initSocket(static_cast<uint16_t>(UDP_port),
//         System::Network::ISocket::CONNECT, ip);
//     NetClient cli;
//     cli.tcpSocket.initSocket(TCP_port, System::Network::ISocket::SERVE);
//     addClient(cli);
//     std::thread(&Networking::runReadThread, this).detach();
//     std::thread(&Networking::runWriteThread, this).detach();
// }

// void Networking::sendToAll(
//     System::Network::ISocket::Type socketType, const std::string &buffer)
// {
//     for (auto &client : _clients) {
//         writeToClient(client.second, buffer, socketType);
//     }
// }

// void Networking::sendToOthers(size_t except_id,
//     System::Network::ISocket::Type socketType, const std::string &buffer)
// {
//     for (auto &client : _clients) {
//         if (client.first != except_id)
//             writeToClient(client.second, buffer, socketType);
//     }
// }

// void Networking::sendToOne(size_t id,
//     System::Network::ISocket::Type socketType, const std::string &buffer)
// {
//     for (auto &client : _clients) {
//         if (client.first == id)
//             writeToClient(client.second, buffer, socketType);
//     }
// }

// ssize_t Networking::authenticateUDPClient(
//     const System::Network::byteArray &packet)
// {
//     std::string decoded = System::Network::decodeString(packet);
//     if (decoded.starts_with(std::to_string(C_START_UDP))) {
//         std::string::size_type p = decoded.find(' ');
//         auto id = std::strtol(&decoded[p], nullptr, 10);
//         if (id == LLONG_MAX || id == LLONG_MIN)
//             return (-1);
//         return (id);
//     }
//     return (-1);
// }

// void serializeString(const std::string &str, std::ostream &out)
// {
//     size_t size = str.size();
//     out.write(reinterpret_cast<const char *>(&size), sizeof(size));
//     out.write(str.data(), static_cast<std::streamsize>(size));
// }

// void Networking::writeToClient(Networking::NetClient &client,
//     const std::string &data, System::Network::ISocket::Type socketType)
// {
//     std::ostringstream out;
//     serializeString(data, out);
//     std::string serializedData = out.str();
//     std::unique_lock lock(_writeMutex);

//     if (socketType == System::Network::ISocket::TCP) {
//         client.writeBufferTCP += serializedData;
//     }
//     if (socketType == System::Network::ISocket::UDP) {
//         client.writeBufferUDP += serializedData;
//     }
//     _writeCondition.notify_one();
// }
