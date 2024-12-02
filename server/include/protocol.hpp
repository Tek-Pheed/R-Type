/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** protocol
*/

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

enum player {
    P_CONN = 101,
    P_POS = 102,
    P_DEAD = 103,
    P_SHOOT = 104,
    P_DMG = 105,
    P_DISCONN = 106,
};

enum enemy {
    E_SPAWN = 111,
    E_POS = 112,
    E_DEAD = 113,
    E_SHOOT = 114,
    E_DMG = 115,
};

enum terrain {
    T_SPAWN = 121,
    T_DEAD = 122,
};

enum mechs {
    M_WAVE = 131,
    M_WOVER = 132,
    M_WIN = 133,
    M_LOSE = 134,
    M_GOVER = 135,
};

#endif /* SERVER_HPP */
