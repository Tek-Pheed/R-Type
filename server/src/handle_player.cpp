/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** handle_player
*/

#include <string>
#include <vector>
#include "protocol.hpp"
#include "server.hpp"

void server::send_to_all(const std::string &message)
{
    for (auto &client : _clients) {
        writeToClient(client.second, message, System::Network::ISocket::UDP);
    }
}

void server::send_to_others(const std::string &message, size_t except_clientID)
{
    for (auto &client : _clients) {
        if (client.first != except_clientID)
            writeToClient(
                client.second, message, System::Network::ISocket::UDP);
    }
}

void server::send_to_one(const std::string &message, size_t clientID)
{
    for (auto &client : _clients) {
        if (client.first == clientID)
            writeToClient(
                client.second, message, System::Network::ISocket::UDP);
    }
}


int server::handle_player(int code, const std::vector<std::string> &tokens)
{
    int id;
    float x,y;
    std::cout << "Handle player" << std::endl;
    switch (code) {
        case P_POS:
        if (tokens.size() <= 2) {
            std::cerr << "Ignoring Invalid Packet" << std::endl;
        }
            id = std::stoi(tokens[0]);
            x = std::stof(tokens[1]);
            y = std::stof(tokens[2]);
            playerPosition(id, x, y);
            // handle_p_pos(tokens);
            break;
        case P_SHOOT:
            id = std::stoi(tokens[0]);
            x = std::stof(tokens[1]);
            y = std::stof(tokens[2]);
            playerShooting(id, x, y);
            break;
        // case P_DISCONN:
        //     id = std::stoi(tokens[0]);
        //     playerDisconnection(id);
        //     break;
        default: break;
    }
    return 0;
}
