/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#include "client.hpp"
#include "system_network.hpp"

int main(void)
{
    client c;
    System::Network::initNetwork();

    // To be set to user input later
    c.create_connection("127.0.0.1", 8081, 8082);

    return (0);
}
