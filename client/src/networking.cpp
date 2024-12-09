/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** networking
*/

#include <sstream>
#include <string>
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "client.hpp"
#include "protocol.hpp"

client::client()
{
    _id = -1;
    _clientSocketTCP = System::Network::TCPSocket();
    _clientSocketUDP = System::Network::UDPSocket();
}

client::~client()
{
    _id = -1;
    _clientSocketTCP.closeSocket();
    _clientSocketUDP.closeSocket();
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
    if (code >= C_INIT_UDP && code <= C_START_UDP)
        return 9;
    return -1;
}

int client::manage_buffer(std::string buffer)
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
        case 0: handle_player(code_int, tokens);
        case 1: handle_enemy(code_int, tokens); break;
        case 2: handle_terrain(code_int, tokens); break;
        case 3: handle_mechs(code_int, tokens); break;
        case 9: handle_connection(code_int, tokens); break;
        default: break;
    }
    return 0;
}

void client::writeToServer(
    std::string &data, System::Network::ISocket::Type socketType)
{
    if (socketType == System::Network::ISocket::TCP) {
        _clientSocketTCP.sendData(System::Network::encodeString(data.c_str()));
    } else {
        _clientSocketUDP.sendData(System::Network::encodeString(data.c_str()));
    }
}

void client::receive_message()
{
    System::Network::byteArray vectTCP;
    System::Network::byteArray vectUDP;

    vectTCP = _clientSocketTCP.receive();
    vectUDP = _clientSocketUDP.receive();
    if (vectTCP.size() > 0) {
        std::string buffer = System::Network::decodeString(vectTCP);
        while (buffer.size() > 0 && buffer[buffer.size() - 1] != '\n'
            && buffer[buffer.size() - 2] != '\t') {
            buffer +=
                System::Network::decodeString(_clientSocketTCP.receive());
        }
        manage_buffer(buffer);
    }
    if (vectUDP.size() > 0) {
        std::string buffer = System::Network::decodeString(vectUDP);
        while (buffer.size() > 0 && buffer[buffer.size() - 1] != '\n'
            && buffer[buffer.size() - 2] != '\t') {
            buffer +=
                System::Network::decodeString(_clientSocketUDP.receive());
        }
        manage_buffer(buffer);
    }
    return;
}

int client::create_connection(const char *ip, int portTCP, int portUDP)
{
    _clientSocketTCP.initSocket(
        static_cast<uint16_t>(portTCP), System::Network::ISocket::CONNECT, ip);
    _clientSocketUDP.initSocket(static_cast<uint16_t>(portUDP), ip);
    return 0;
}
