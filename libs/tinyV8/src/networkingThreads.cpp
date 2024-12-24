// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** networkingThreads
// */

// #include <iostream>
// #include <mutex>
// #include <string>
// #include "EngineNetworking.hpp"

// using namespace Engine::Feature;

// void NetworkingManager::runConnectThread()
// {
//     System::Network::socketSetGeneric readfds;

//     while (true) {
//         readfds.clear();
//         readfds.emplace_back(&_SocketTCP);
//         try {
//             System::Network::select(&readfds);
//         } catch (const std::exception &s) {
//             std::cout << "An error occured on the server: " << s.what()
//                       << std::endl;
//         }
//         if (readfds.size() == 0)
//             continue;
//         NetClient cl;
//         cl.tcpSocket = System::Network::accept(_SocketTCP);
//         addClient(cl);
//         // std::string toWrite = makePacket(C_INIT_UDP, _clientCounter);
//         // writeToClient(client, toWrite, System::Network::ISocket::TCP);
//     }
// }

// void NetworkingManager::runWriteThread()
// {
//     System::Network::socketSetGeneric writefds;
//     System::Network::timeoutStruct tv = {{0, 50000}};
//     bool shouldWait = true;

//     while (true) {
//         try {
//             if (shouldWait) {
//                 std::unique_lock lock(_writeMutex);
//                 _writeCondition.wait(lock);
//             }
//             writefds.clear();
//             writefds.emplace_back(&_SocketUDP);
//             for (auto &pair : _clients) {
//                 NetClient &ref = pair.second;
//                 if ((ref.writeBufferTCP.length() > 0
//                         || ref.writeBufferUDP.length() > 0)
//                     && !ref.isDisconnected)
//                     writefds.emplace_back(&ref.tcpSocket);
//             }
//             System::Network::select(nullptr, &writefds, nullptr, tv);
//             shouldWait = true;
//             if (writefds.size() == 0)
//                 continue;
//             for (auto sock : writefds) {
//                 ssize_t len = 0;
//                 int removeID = -1;
//                 bool can_remove = false;
//                 switch (sock->getType()) {
//                     case System::Network::ISocket::TCP: {
//                         ssize_t id = this->identifyClient(*sock);
//                         if (id == -1)
//                             std::cout << "Unable to idendify client (TCP - "
//                                          "write thread)"
//                                       << std::endl;
//                         NetClient &client = this->getClient((size_t) id);
//                         if (client.isDisconnected) {
//                             removeID = (int) id;
//                             if (client.writeBufferTCP.length() == 0
//                                 && client.writeBufferUDP.length() == 0)
//                                 can_remove = true;
//                         }
//                         if (!sock->isOpen()) {
//                             std::cout << "[Write Thread] client ("
//                                       << std::to_string(id)
//                                       << ") closed TCP connection ("
//                                       << std::to_string(sock->getUID()) << ")"
//                                       << std::endl;
//                             this->removeClient((size_t) id);
//                         }
//                         if (client.writeBufferTCP.size() > 0) {
//                             len = sock->sendData(System::Network::encodeString(
//                                 client.writeBufferTCP));
//                             std::cout << "[Write Thread] Sending: "
//                                       << client.writeBufferTCP.substr(
//                                              0, (size_t) len)
//                                       << "to client (" << std::to_string(id)
//                                       << ") on TCP connection ("
//                                       << std::to_string(sock->getUID()) << ")"
//                                       << std::endl;
//                             if ((size_t) len < client.writeBufferTCP.length())
//                                 shouldWait = false;
//                             client.writeBufferTCP.erase(0, (size_t) len);
//                         }
//                         break;
//                     }
//                     case System::Network::ISocket::UDP: {
//                         if (sock->getUID() == this->_SocketUDP.getUID()) {
//                             for (auto &cl : _clients) {
//                                 size_t index = cl.first;
//                                 auto &cli = cl.second;
//                                 if (cli.isDisconnected) {
//                                     removeID = (int) index;
//                                     if (cli.writeBufferTCP.length() == 0
//                                         && cli.writeBufferUDP.length() == 0)
//                                         can_remove = true;
//                                 }
//                                 if (cli.port == 0 || cli.ip.empty()
//                                     || cli.writeBufferUDP.length() == 0)
//                                     continue;
//                                 len = _SocketUDP.sendDataTo(
//                                     System::Network::encodeString(
//                                         cli.writeBufferUDP),
//                                     cli.ip, cli.port);
//                                 std::cout
//                                     << "[Write Thread] Sending: "
//                                     << cli.writeBufferUDP.substr(
//                                            0, (size_t) len)
//                                     << "to client (" << std::to_string(index)
//                                     << ") on UDP connection ("
//                                     << std::to_string(sock->getUID()) << ")"
//                                     << std::endl;
//                                 if ((size_t) len < cli.writeBufferUDP.length())
//                                     shouldWait = false;
//                                 cli.writeBufferUDP.erase(0, (size_t) len);
//                             }
//                         }
//                     }
//                     default: {
//                         break;
//                     };
//                 }
//                 if (removeID != -1 && can_remove == true) {
//                     std::unique_lock lock(_globalMutex);
//                     _clients.erase((size_t) removeID);
//                 }
//             }
//         } catch (const std::exception &e) {
//             std::cout << "[Write Thread] failed with exception: " << e.what()
//                       << std::endl;
//         }
//     }
// }

// void NetworkingManager::runReadThread()
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
//                             client.readBufferTCP.push_back(buffer);
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
