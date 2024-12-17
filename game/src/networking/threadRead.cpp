// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** thread_read
// */

// #if defined(WIN32)
//     #define NOMINMAX
// #endif

// #include <climits>
// #include <cstddef>
// #include <cstdlib>
// #include <exception>
// #include <iostream>
// #include <mutex>
// #include <string>
// #include <sys/types.h>
// #include "Networking.hpp"
// #include "system_network.hpp"
// #include "system_tcp.hpp"
// #include "system_udp.hpp"

// void Networking::runReadThread()
// {
//     System::Network::byteArray arr;
//     System::Network::byteArray vect;
//     System::Network::socketSetGeneric readfds;
//     System::Network::timeoutStruct tv = {{0, 500000}};

//     while (true) {
//         try {
//             readfds.clear();
//             this->_globalMutex.lock();
//             readfds.emplace_back(&_SocketUDP);
//             for (auto &pair : _clients) {
//                 NetClient &ref = pair.second;
//                 if (!ref.isDisconnected)
//                     readfds.emplace_back(&ref.tcpSocket);
//             }
//             this->_globalMutex.unlock();
//             System::Network::select(&readfds, nullptr, nullptr, tv);
//             if (readfds.size() == 0)
//                 continue;
//             for (auto sock : readfds) {
//                 vect.clear();
//                 arr.clear();
//                 switch (sock->getType()) {
//                     case System::Network::ISocket::TCP: {
//                         ssize_t id = 1;
//                         if (_isServer) {
//                             id = identifyClient(*sock);
//                             if (id == -1)
//                                 std::cout
//                                     << "Unable to idendify client (TCP - "
//                                        "read thread)"
//                                     << std::endl;
//                         }
//                         if (!sock->isOpen()) {
//                             std::cout << "[Read Thread] client ("
//                                       << std::to_string(id)
//                                       << ") closed TCP connection ("
//                                       << std::to_string(sock->getUID()) << ")"
//                                       << std::endl;
//                             if (_isServer)
//                                 removeClient((size_t) id);
//                         } else {
//                             NetClient &client = this->getClient((size_t) id);
//                             if (client.isDisconnected)
//                                 continue;
//                             vect = sock->receive();
//                             std::string buff =
//                                 System::Network::decodeString(vect);
//                             std::cout
//                                 << "[Read Thread] Message received on TCP ("
//                                 << std::to_string(sock->getUID())
//                                 << ") for client (" << std::to_string(id)
//                                 << "): " << buff << std::endl;
//                             std::unique_lock lock(_globalMutex);
//                             std::string buffer =
//                                 System::Network::decodeString(vect);
//                             while (buffer.size() > 0
//                                 && buffer[buffer.size() - 1] != '\n'
//                                 && buffer[buffer.size() - 2] != '\t') {
//                                 buffer += System::Network::decodeString(
//                                     client.tcpSocket.receive());
//                             }
//                             std::cout << "Received TCP: " << buffer
//                                       << std::endl;
//                             _globalMutex.lock();
//                             client.readBufferTCP.push_back(buffer);
//                             _globalMutex.unlock();
//                         }
//                         break;
//                     }
//                     case System::Network::ISocket::UDP: {
//                         std::string addr;
//                         uint16_t port;
//                         System::Network::UDPSocket *s =
//                             static_cast<System::Network::UDPSocket *>(sock);
//                         if (s == nullptr)
//                             break;
//                         vect = s->receiveFrom(addr, port);
//                         ssize_t id = 1;
//                         if (_isServer)
//                             id = identifyClient(addr, std::to_string(port));
//                         if (id == -1) {
//                             id = authenticateUDPClient(vect);
//                             if (id == -1) {
//                                 std::cout
//                                     << "[Read Thread] Failed to identify "
//                                        "client on UDP ("
//                                     << std::to_string(sock->getUID())
//                                     << ") with packet: "
//                                     << System::Network::decodeString(vect)
//                                     << std::endl;
//                                 continue;
//                             } else {
//                                 std::cout << "[Read Thread] Successfully "
//                                              "authenticated client ("
//                                           << std::to_string(id)
//                                           << ") on UDP Socket ("
//                                           << std::to_string(sock->getUID())
//                                           << ")" << std::endl;
//                                 NetClient &client = getClient((size_t) id);
//                                 if (client.isDisconnected)
//                                     continue;
//                                 client.ip = addr;
//                                 client.port = port;
//                             }
//                         }
//                         NetClient &client = getClient((size_t) id);
//                         if (client.isDisconnected)
//                             continue;
//                         std::string buff = System::Network::decodeString(vect);
//                         std::cout << "[Read Thread] Message received on UDP ("
//                                   << std::to_string(sock->getUID())
//                                   << ") for client (" << std::to_string(id)
//                                   << "): " << buff << std::endl;
//                         _globalMutex.lock();
//                         std::string buffer =
//                             System::Network::decodeString(vect);
//                         while (buffer.size() > 0
//                             && buffer[buffer.size() - 1] != '\n'
//                             && buffer[buffer.size() - 2] != '\t') {
//                             buffer += System::Network::decodeString(
//                                 _SocketUDP.receive());
//                         }
//                         client.readBufferUDP.push_back(buffer);
//                         _globalMutex.unlock();
//                         break;
//                     }
//                     default: {
//                         break;
//                     }
//                 }
//             }
//         } catch (const std::exception &e) {
//             std::cout << "[Read Thread] failed with exception: " << e.what()
//                       << std::endl;
//         }
//     }
// }
