/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player
*/

#include "client.hpp"
#include "protocol.hpp"

void client::handle_player(int code, std::vector<std::string> &tokens)
{
    (void) tokens;
    switch (code) {
        case P_SETUP: {
            break;
        }
        case P_CONN: {
            break;
        }
        case P_POS: {
            break;
        }
        case P_DEAD: {
            break;
        }
        case P_SHOOT: {
            break;
        }
        case P_DMG: {
            break;
        }
        case P_DISCONN: {
            break;
        }
        default: {
            break;
        }
    }
}
