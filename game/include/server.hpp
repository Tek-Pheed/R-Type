/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include <memory>
#include <mutex>

#include "Entity.hpp"
#include "RenderClass.hpp"
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"
#include "Networking.hpp"

#define SERVER_TPS 30

void serverLoop(Networking &net, game &game);

#endif /*SERVER_HPP*/