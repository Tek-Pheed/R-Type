/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#include "server.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <bits/stdc++.h>

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "protocol.hpp"
#include "server.hpp"

#define PORT    8081
#define MAXLINE 1400

server::server() : _sockupd()
{
}

server::~server()
{
}

void server::create_server()
{
    _sockupd.initSocket(PORT);
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

int manage_buffer(std::string buffer, ssize_t n)
{
    std::string code = std::string(buffer).substr(0, 3);
    int i_code = atoi(code.c_str());
    int code_pro = is_code_valid(atoi(code.c_str()));
    std::vector<std::string> tokens;

    if (code_pro == -1) {
        return -1;
    }
    std::string str = buffer.substr(4, static_cast<size_t>(n) - 4);
    std::istringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    /*switch (code_pro) {
        case 0: handle_player(i_code, tokens); break;
        default: break;
    }*/
    return 0;
}

void server::receive_message()
{
    System::Network::byteArray arr;
    std::string buffer;

    if (!_sockupd.isOpen())
        return;
    System::Network::socketSetGeneric readfds;
    System::Network::socketSetGeneric writefds;

    readfds.emplace_back(&_sockupd);
    // writefds.emplace_back(&_sockupd);
    System::Network::select(&readfds, &writefds, nullptr);

    for (auto &sock : readfds) {
        auto vect = sock->receive();
        buffer = System::Network::decodeString(vect);
        if (buffer[buffer.size() - 1] == '\n'
            && buffer[buffer.size() - 2] == '\t') {
            arr.insert(arr.end(), vect.begin(), vect.end());
            break;
        }
    }

    printf(
        "Message received: %s\n", System::Network::decodeString(arr).c_str());
    manage_buffer(buffer, static_cast<ssize_t>(buffer.size()));
    return;
}

int main()
{
    server s;
    s.create_server();
    System::Network::initNetwork();

    while (true) {
        s.receive_message();
    }
    // System::Network::stopNetwork();
}
