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
    int id, amount;

    switch (code) {
        case P_POS:
            if (tokens.size() <= 2) {
                std::cerr << "Ignoring Invalid Packet" << std::endl;
                break;
            }
            float x, y;
            id = std::stoi(tokens[0]);
            x = std::stof(tokens[1]);
            y = std::stof(tokens[2]);
            playerPosition(id, x, y);
            break;

        case P_SHOOT:
            if (tokens.size() == 0) {
                std::cerr << "Ignoring Invalid Packet" << std::endl;
                break;
            }
            id = std::stoi(tokens[0]);
            playerShooting(id);
            break;

        case P_DMG:
            if (tokens.size() <= 1) {
                std::cerr << "Ignoring Invalid Packet" << std::endl;
                break;
            }
            id = std::stoi(tokens[0]);
            amount = std::stoi(tokens[1]);
            playerDamaged(id, amount);
            break;

        default:
            break;
    }
    return 0;
}
