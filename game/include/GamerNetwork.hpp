/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#ifndef GAME_NETWORK_HPP
#define GAME_NETWORK_HPP

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "EngineNetworking.hpp"
#include "Game.hpp"

namespace RType
{
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
    };

} // namespace RType

#endif /* GAME_NETWORK_HPP */