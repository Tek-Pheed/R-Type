/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** connection
*/

#include <sstream>
#include <string>

#include "game.hpp"

void setIdPlayer(game &game)
{
    for (auto &entity : game.getEntities()) {
        if (entity->getID() == static_cast<std::size_t>(-1)) {
            entity->setID(static_cast<std::size_t>(game.getId()));
            break;
        }
    }
}

void game::handleConnection(int code, std::vector<std::string> &tokens)
{
    if (code == 901) {
        _id = atoi(tokens[0].c_str());
        setIdPlayer(*this);
        std::stringstream ss;
        ss << "902 " << tokens[0] << "\t\f";
        std::string formattedString = ss.str();
        writeToServer(formattedString, System::Network::ISocket::UDP);
    }
    return;
}
