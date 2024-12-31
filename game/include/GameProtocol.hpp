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
        void serializeString(
            const std::string &str, std::ostream &out) override;
        void deserializeString(
            const std::ostream &in, std::string &out) override;
    };

    std::string serverInitUDP(size_t id);
    std::string clientStartUDP(size_t id);
    std::string serverConfirmUDP(bool isOk);
    std::string playerConnection(size_t playerID, float x, float y);
} // namespace RType

#endif /* SERVER_HPP */
