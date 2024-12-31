/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** protocolImplement
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <sstream>
#include <string>
#include "GameProtocol.hpp"
#include "Game.hpp"

using namespace RType;

int GameInstance::is_code_valid(int code)
{
    if (code >= P_CONN && code <= P_DISCONN)
        return 0;
    if (code >= E_SPAWN && code <= E_DMG)
        return 1;
    if (code >= T_SPAWN && code <= T_DEAD)
        return 2;
    if (code >= M_WAVE && code <= M_GOVER)
        return 3;
    if (code >= C_INIT_UDP && code <= C_AUTH)
        return 9;
    return -1;
}

PacketHandler::PacketHandler()
{
}

PacketHandler::~PacketHandler()
{
}

// This will need to be more robust
std::vector<std::string> PacketHandler::splitPackets(
    const System::Network::byteArray &bytes, size_t &resultIndexEnd)
{
    std::vector<std::string> out;
    size_t start = 0;

    if (bytes.size() > 1) {
        std::string buffer = System::Network::decodeString(bytes);

        for (size_t i = 1; i < buffer.size(); i++) {
            if (buffer[i - 1] == PACKET_END[0] && buffer[i] == PACKET_END[1]) {
                out.emplace_back(buffer.substr(start, i + 1));
                start = i + 1;
            }
        }
    }
    resultIndexEnd = start;
    return (out);
}

// This will need to be more robust
ssize_t PacketHandler::identifyClient(const System::Network::byteArray &bytes)
{
    size_t end = 0;
    std::vector<std::string> packets = splitPackets(bytes, end);

    for (const auto &decoded : packets) {
        if (decoded.starts_with(std::to_string(C_START_UDP))) {
            std::string::size_type p = decoded.find(' ');
            auto id = std::strtol(&decoded[p], nullptr, 10);
            if (id == LLONG_MAX || id == LLONG_MIN)
                return (-1);
            return (id);
        }
    }
    return (-1);
}

// This will need to be reworked
void PacketHandler::serializeString(const std::string &str, std::ostream &out)
{
    // size_t size = str.size();
    // out.write(reinterpret_cast<const char *>(&size), sizeof(size));
    // out.write(str.data(), static_cast<std::streamsize>(size));
    out.write(str.data(), static_cast<std::streamsize>(str.size()));
}

// This will need to be reworked
// Note: for now this function is unused be the interface function should be
// called in the game engine networking threads.
void PacketHandler::deserializeString(const std::ostream &in, std::string &out)
{
    std::stringstream ss;
    ss << in.rdbuf();
    out = ss.str();
}

template <> std::string RType::makePacket(int protocolCode)
{
    std::string pack = std::to_string(protocolCode);

    pack += PACKET_END;
    return (pack);
}