/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#include "server.hpp"
#include <atomic>
#include <csignal>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "system.hpp"
#include "system_tcp.hpp"

static volatile std::atomic<bool> stop = false;

//void sighandler(int n)
//{
//    (void) n;
//    stop = true;
//}

// Example TCP Server with libSystem
int main(void)
{
    System::Network::TCPSocket serverSocket(
        1234, System::Network::TCPSocket::SERVE);

    std::vector<System::Network::TCPSocket> clients;

    System::Network::socketSetTCP socketSetTCP;
    System::Network::timeoutStruct tv;
    tv = {1, 0};
    clients.clear();
    //signal(SIGINT, &sighandler);

    try {
        std::cout << "Started TCP Server on Port 1234" << std::endl;

        while (!stop) {
            socketSetTCP.clear();
            System::Network::addSocketToSet(&serverSocket, socketSetTCP);
            System::Network::addSocketToSet(clients, socketSetTCP);

            std::cout << "Set size: " + std::to_string(socketSetTCP.size())
                      << std::endl;
            System::Network::select(&socketSetTCP, nullptr, nullptr, tv);

            std::cout << "Select event" << std::endl;

            for (auto &sock : socketSetTCP) {
                if (sock == nullptr) {
                    continue;
                }
                if (sock == &serverSocket) {
                    clients.emplace_back(
                        System::Network::accept(*socketSetTCP[0]));
                    std::cout << "New client connected" << std::endl;
                } else {
                    std::cout << "Reading from client: "
                              << System::Network::decodeString(sock->receive())
                              << std::endl;
                    //sock->send(System::Network::byteArray({'h','e', 'l', 'l', 'o'}));
                    if (!sock->isOpen()) {
                        std::cout << "Removing client" << std::endl;
                        sock->closeSocket();
                        System::Network::removeSocketInSet(*sock, socketSetTCP);
                        System::Network::removeSocketInVect(*sock, clients);
                    }
                }
            }
        }
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }

    std::cout << "Stopping Server" << std::endl;
    serverSocket.closeSocket();
    return (0);
}
