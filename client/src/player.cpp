/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player
*/

#include <sstream>

#include "Components.hpp"
#include "RenderClass.hpp"
#include "client.hpp"

ecs::Entity &client::getLocalPlayer()
{
    return _player;
}

void client::update_localplayer_position()
{
    auto position = _player.getComponent<ecs::PositionComponent>();
    if (position) {
        float oldX = position->getOldX();
        float oldY = position->getOldY();
        float x = position->getX();
        float y = position->getY();

        if (oldX != x || oldY != y) {
            std::stringstream ss;
            ss << "102 " << _id << " " << x << " " << y << "\t\n";
            writeToServer(ss.str(), System::Network::ISocket::UDP);
        }
    }
}
