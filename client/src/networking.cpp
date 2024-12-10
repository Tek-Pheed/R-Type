/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** networking
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

#include "client.hpp"
#include "protocol.hpp"

client::client(RenderClass &render)
    : _clientSocketTCP(System::Network::TCPSocket()),
      _clientSocketUDP(System::Network::UDPSocket()), _id(-1),
      _refRender(render)
{
    return;
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
    if (code >= C_INIT_UDP && code <= C_AUTH)
        return 9;
    return -1;
}

int client::manage_buffers()
{
    std::unique_lock lock(_mutex);
    if (_buffers.size() == 0)
        return 0;
    for (auto buffer : _buffers) {
        std::string codeStr = std::string(buffer).substr(0, 3);
        int code = atoi(codeStr.c_str());
        int code_int = is_code_valid(code);
        std::vector<std::string> tokens;
        if (code_int == -1) {
            return -1;
        }
        std::string str = buffer.substr(4, buffer.size() - 4);
        std::istringstream ss(str);
        std::string token;
        std::cout << "Buffer: " << buffer << std::endl;
        while (std::getline(ss, token, ' ')) {
            tokens.push_back(token);
        }
        switch (code_int) {
            case 0: handle_player(code, tokens); break;
            case 1: handle_enemy(code, tokens); break;
            case 2: handle_terrain(code, tokens); break;
            case 3: handle_mechs(code, tokens); break;
            case 9: handle_connection(code, tokens); break;
            default: break;
        }
        auto it =
            std::find(_buffers.begin(), _buffers.end(), std::string(buffer));
        if (it != _buffers.end()) {
            _buffers.erase(it);
        }
    }
    return 0;
}

void client::writeToServer(
    const std::string &data, System::Network::ISocket::Type socketType)
{
    if (socketType == System::Network::ISocket::TCP) {
        std::cout << "Sending TCP: " << data << std::endl;
        _clientSocketTCP.sendData(System::Network::encodeString(data));
    } else {
        _clientSocketUDP.sendData(System::Network::encodeString(data));
    }
}

void client::receive_message()
{
    System::Network::byteArray vect;
    System::Network::byteArray arr;
    System::Network::socketSetGeneric readfds;

    while (true) {
        readfds.clear();
        readfds.emplace_back(&_clientSocketTCP);
        readfds.emplace_back(&_clientSocketUDP);
        System::Network::select(&readfds, nullptr, nullptr);
        if (readfds.size() == 0)
            continue;
        for (auto sock : readfds) {
            vect.clear();
            arr.clear();
            switch (sock->getType()) {
                case System::Network::ISocket::TCP: {
                    vect = _clientSocketTCP.receive();
                    if (vect.size() > 0) {
                        std::string buffer =
                            System::Network::decodeString(vect);
                        while (buffer.size() > 0
                            && buffer[buffer.size() - 1] != '\n'
                            && buffer[buffer.size() - 2] != '\t') {
                            buffer += System::Network::decodeString(
                                _clientSocketTCP.receive());
                        }
                        std::cout << "Received TCP: " << buffer << std::endl;
                        _mutex.lock();
                        _buffers.push_back(buffer);
                        _mutex.unlock();
                    }
                    break;
                }
                case System::Network::ISocket::UDP: {
                    vect = _clientSocketUDP.receive();
                    if (vect.size() > 0) {
                        std::string buffer =
                            System::Network::decodeString(vect);
                        while (buffer.size() > 0
                            && buffer[buffer.size() - 1] != '\n'
                            && buffer[buffer.size() - 2] != '\t') {
                            buffer += System::Network::decodeString(
                                _clientSocketUDP.receive());
                        }
                        std::cout << "Received UDP: " << buffer << std::endl;
                        _mutex.lock();
                        _buffers.push_back(buffer);
                        _mutex.unlock();
                    }
                    break;
                };
                default: break;
            }
        }
    }
}

int client::create_connection(const char *ip, int portTCP, int portUDP)
{
    try {
        _clientSocketTCP.initSocket(static_cast<uint16_t>(portTCP),
            System::Network::ISocket::CONNECT, ip);
        _clientSocketUDP.initSocket(static_cast<uint16_t>(portUDP),
            System::Network::ISocket::CONNECT, ip);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}
