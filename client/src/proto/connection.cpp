/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** connection
*/

#include <sstream>
#include <string>

#include "client.hpp"

void client::handle_connection(int code, std::vector<std::string> tokens)
{
    if (code == 901) {
        _id = atoi(tokens[0].c_str());
        std::stringstream ss;
        ss << "902 " << tokens[0] << "\t\f";
        std::string formattedString = ss.str();
        writeToServer(formattedString, System::Network::ISocket::UDP);
    }
    return;
}
