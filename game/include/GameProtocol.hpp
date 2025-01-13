/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** R-Type protocol
*/

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <string>
#include "EngineNetworking.hpp"
namespace RType
{

    constexpr auto PACKET_END = "\t\n";

    enum Protocol {
        // Connection
        C_INIT_UDP = 901,
        C_START_UDP = 902,
        C_AUTH = 903,

        // Player
        P_CONN = 101,
        P_POS = 102,
        P_DEAD = 103,
        P_SHOOT = 104,
        P_DMG = 105,
        P_DISCONN = 106,
        P_NAME = 107,

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
        M_MUSIC = 136,
        M_BG = 137,

        // Boss
        B_SPAWN = 141,
        B_POS = 142,
        B_DEAD = 143,
        B_SHOOT = 144,
        B_DMG = 145,

        // Loby
        L_STARTGAME = 241,
        L_SETMAXPLAYRS = 242,
    };

    template <typename... Args>
    std::string makePacket(int protocolCode, Args... args)
    {
        std::string pack = std::to_string(protocolCode);

        for (const auto &ag : {args...}) {
            pack += " " + getString(ag);
        }
        pack += PACKET_END;
        return (pack);
    }

    class PacketHandler : public Engine::IPacketManager {
      public:
        PacketHandler();
        ~PacketHandler();

        std::vector<std::string> splitPackets(
            const System::Network::byteArray &bytes,
            size_t &resultIndexEnd) override;
        ssize_t identifyClient(
            const System::Network::byteArray &bytes) override;
        void obfuscateData(char *data, size_t size, char key);
        void serializeString(
            const std::string &str, std::ostream &out, char key) override;
        std::string deserializeString(
            std::istream &in, char key, size_t size) override;
        char getKey(void) const override;
    };

} // namespace RType

#endif /* SERVER_HPP */
