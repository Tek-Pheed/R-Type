/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** R-Type protocol
*/

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

namespace RType
{
    constexpr auto PACKET_END = "\t\n";

    enum Protocol {
        // Connection
        C_INIT_UDP = 901,
        C_START_UDP = 902,
        C_AUTH = 903,

        // Player
        P_SETUP = 100,
        P_CONN = 101,
        P_POS = 102,
        P_DEAD = 103,
        P_SHOOT = 104,
        P_DMG = 105,
        P_DISCONN = 106,

        // Enemy
        E_SPAWN = 111,
        E_POS = 112,
        E_DEAD = 113,
        E_SHOOT = 114,
        E_DMG = 115,

        // Terrain
        T_SPAWN = 121,
        T_DEAD = 122,

        // Mechs
        M_WAVE = 131,
        M_WOVER = 132,
        M_WIN = 133,
        M_LOSE = 134,
        M_GOVER = 135,
    };

} // namespace RType

#endif /* SERVER_HPP */
