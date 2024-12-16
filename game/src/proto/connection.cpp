/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** connection
*/

#include <sstream>
#include <string>

#include "game.hpp"

void set_id_player(game &game)
{
    for (auto &entity : game.get_entities()) {
        if (entity->getID() == static_cast<std::size_t>(-1)) {
            entity->setID(static_cast<std::size_t>(game.get_id()));
            break;
        }
    }
}

void game::handle_connection(int code, std::vector<std::string> &tokens)
{
    if (code == 901) {
        _id = atoi(tokens[0].c_str());
        set_id_player(*this);
        std::stringstream ss;
        ss << "902 " << tokens[0] << "\t\f";
        std::string formattedString = ss.str();
        writeToServer(formattedString, System::Network::ISocket::UDP);
    }
    return;
}
