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

std::shared_ptr<ecs::Entity> &client::getLocalPlayer()
{
    return _player;
}

void client::update_localplayer_position()
{
    auto position = _player->getComponent<ecs::PositionComponent>();

    if (position) {
        float x = position->getX();
        float y = position->getY();

        std::stringstream ss;
        ss << "102 " << _id << " " << x << " " << y << "\t\n";
        writeToServer(ss.str(), System::Network::ISocket::UDP);
    }
}
