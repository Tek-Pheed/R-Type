/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server specific functions
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cstddef>
#include "GameNetwork.hpp"
#include "GameProtocol.hpp"
#include "system_network.hpp"

using namespace RType;

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
                out.emplace_back(buffer.substr(start, i));
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

// This will need to reworked
void PacketHandler::serializeString(const std::string &str, std::ostream &out)
{
    size_t size = str.size();
    out.write(reinterpret_cast<const char *>(&size), sizeof(size));
    out.write(str.data(), static_cast<std::streamsize>(size));
}

void GameInstance::setupServer(uint16_t tcpPort, uint16_t udpPort)
{
    _isServer = true;
    _tcpPort = tcpPort;
    _udpPort = udpPort;
    refNetworkManager.setupServer<PacketHandler>(_tcpPort, _udpPort);
}
