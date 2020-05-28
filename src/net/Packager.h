#pragma once

#include <cstring>
#include <array>
#include <memory>

#include <SFML/Network/Packet.hpp>

#include "Connection.h"
#include "ParsedPacket.h"
#include "PacketType.h"
#include "Log.h"

class Packager
{
public:
    static sf::Packet Pack(PacketType type, const sf::Uint8 *data, size_t size) noexcept;
    template <typename S>
    static std::optional<ParsedPacket> Parse(sf::Packet packet, const Connection<S> *conn) noexcept;

    static PacketType GetTypeFromRawPacket(const sf::Packet &rawPacket) noexcept;
};

template <typename S>
std::optional<ParsedPacket> Packager::Parse(sf::Packet packet, const Connection<S> *conn) noexcept
{
    sf::Uint8 *rawPacketData = (sf::Uint8 *)packet.getData();

    ParsedPacket parsedPacket;

    // CHECK SIZES
    parsedPacket.fullSize = packet.getDataSize();
    parsedPacket.dataSize = parsedPacket.fullSize - sizeof(PacketType);
    if (parsedPacket.data < 0)
        return std::nullopt;

    // TYPE
    parsedPacket.type = GetTypeFromRawPacket(packet);
    if (parsedPacket.type < PacketType::Invalid || parsedPacket.type > PacketType::Count)
        return std::nullopt;

    // COPY DATA IF THERE IS ANY
    if (parsedPacket.dataSize)
    {
        parsedPacket.data = std::shared_ptr<sf::Uint8>(new sf::Uint8[parsedPacket.dataSize]);
        memcpy(parsedPacket.data.get(), rawPacketData + sizeof(PacketType), parsedPacket.dataSize);
    }

    parsedPacket.connection = conn;
    parsedPacket.protocol = ProtocolFrom<S>();

    return parsedPacket;
}
