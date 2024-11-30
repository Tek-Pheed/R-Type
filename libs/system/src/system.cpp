/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#include "system.hpp"
#include "system_tcp.hpp"

System::Network::NetworkException::NetworkException(const std::string &str)
    : _content(str)
{
}

const char *System::Network::NetworkException::what() const noexcept
{
    return (this->_content.c_str());
}

void empty(void)
{
    return;
}
