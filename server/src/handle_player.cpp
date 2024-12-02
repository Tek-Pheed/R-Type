/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** handle_player
*/

#include <string>
#include <vector>
#include "protocol.hpp"

int handle_player(int code, std::vector<std::string> tokens)
{
    switch (code) {
        case P_POS:
            // handle_p_pos(tokens);
            break;
        case P_SHOOT:
            // handle_p_shoot(tokens);
            break;
        case P_DISCONN:
            // handle_p_disconn(tokens);
            break;
        default: break;
    }
    return 0;
}
