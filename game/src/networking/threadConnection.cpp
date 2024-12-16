/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** threadConnection
*/

#include <iostream>
#include "Networking.hpp"
#include "protocol.hpp"

void Networking::runConnectThread()
{
    System::Network::socketSetGeneric readfds;

    while (true) {
        readfds.clear();
        readfds.emplace_back(&_serverSocketTCP);
        try {
            System::Network::select(&readfds);
        } catch (const std::exception &s) {
            std::cout << "An error occured on the server: " << s.what()
                      << std::endl;
        }
        if (readfds.size() == 0)
            continue;
        NetClient cl;
        cl.tcpSocket = System::Network::accept(_serverSocketTCP);
        NetClient &client = addClient(cl);
        std::string toWrite = makePacket(C_INIT_UDP, _clientCounter);
        writeToClient(client, toWrite, System::Network::ISocket::TCP);
    }
}
