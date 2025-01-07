/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** protocolImplement
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Game.hpp"
#include "GameProtocol.hpp"

using namespace RType;

int GameInstance::is_code_valid(int code)
{
    if (code >= P_CONN && code <= P_NAME)
        return 0;
    if (code >= E_SPAWN && code <= E_DMG)
        return 1;
    if (code >= T_SPAWN && code <= T_DEAD)
        return 2;
    if (code >= M_WAVE && code <= M_GOVER)
        return 3;
    if (code >= L_STARTGAME && code <= L_SETMAXPLAYRS)
        return 24;
    if (code >= B_SPAWN && code <= B_DMG)
        return 4;
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
        std::istringstream in(buffer);

        while (in) {
            try {
                std::string packet =
                    deserializeString(in, getKey(), buffer.size());
                if (!packet.empty()) {
                    std::cout << "Deserialized: " << packet << std::endl;
                    out.push_back(packet);
                    start = buffer.size();
                }
            } catch (const std::runtime_error &e) {
                std::cerr << "Error deserializing packet: " << e.what()
                          << std::endl;
                break;
            }
        }
    }

    resultIndexEnd = start;
    return out;
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

void PacketHandler::obfuscateData(char *data, size_t size, char key)
{
    for (size_t i = 0; i < size; ++i) {
        data[i] ^= key;
    }
}

// This will need to be reworked
void PacketHandler::serializeString(
    const std::string &str, std::ostream &out, char key)
{
    size_t size = str.size();
    std::cout << "Serializing: " << str << std::endl;
    out.write(reinterpret_cast<const char *>(&size), sizeof(size));

    std::vector<char> buffer(str.begin(), str.end());
    obfuscateData(buffer.data(), size, key);
    out.write(buffer.data(), static_cast<std::streamsize>(size));
}

// This will need to be reworked
// Note: for now this function is unused be the interface function should be
// called in the game engine networking threads.
std::string PacketHandler::deserializeString(
    std::istream &in, char key, size_t size)
{
    size = 0;
    in.clear();

    if (!in.read(reinterpret_cast<char *>(&size), sizeof(size))) {
        return "";
    }

    if (size > static_cast<size_t>(in.rdbuf()->in_avail())) {
        return "";
    }

    std::vector<char> buffer(size);
    if (!in.read(buffer.data(), static_cast<std::streamsize>(size))) {
        return "";
    }

    obfuscateData(buffer.data(), size, key);
    return std::string(buffer.begin(), buffer.end());
}

template <> std::string RType::makePacket(int protocolCode)
{
    std::string pack = std::to_string(protocolCode);

    pack += PACKET_END;
    return (pack);
}

char PacketHandler::getKey(void) const
{
    return 0x1A;
}
