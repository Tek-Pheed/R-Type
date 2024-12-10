/*
** EPITECH PROJECT, 2024
** Untitled (Workspace)
** File description:
** serialize
*/

#include <cstddef>
#include <string>
#include "Components.hpp"
#include "Entity.hpp"
#include "server.hpp"

template <typename componentType>
std::string serializeComponent(componentType Component)
{
    (void) Component;
    return ("");
}

template <> std::string serializeComponent(ecs::PositionComponent Component)
{
    std::string str = "";

    str += std::to_string(Component.getX());
    str += " " + std::to_string(Component.getY());
    return (str);
}

template <> std::string serializeComponent(ecs::VelocityComponent Component)
{
    std::string str = "";

    str += std::to_string(Component.getVx());
    str += " " + std::to_string(Component.getVy());
    return (str);
}

template <> std::string serializeComponent(ecs::HealthComponent Component)
{
    std::string str = "";

    str += std::to_string(Component.getHealth());
    return (str);
}

template <> std::string serializeComponent(ecs::BossComponent Component)
{
    std::string str = "";

    str += Component.getName();
    return (str);
}

template <> std::string serializeComponent(ecs::EnemyComponent Component)
{
    std::string str = "";

    str += Component.getName();
    return (str);
}

template <> std::string serializeComponent(ecs::TextComponent Component)
{
    std::string str = "";

    str += Component.getText();
    return (str);
}

template <> std::string serializeComponent(ecs::PlayerComponent Component)
{
    std::string str = "";

    str += Component.getName();
    return (str);
}

