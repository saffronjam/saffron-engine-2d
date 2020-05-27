#pragma once

#include <cstring>

#include <SFML/Network/Packet.hpp>

#include "Connection.h"
#include "ParsedPacket.h"
#include "PacketType.h"
#include "Log.h"

class Packager
{
public:
    static sf::Packet Pack(PacketType type, const sf::Uint8 *data, size_t size)
    {
        sf::Packet packet;
        packet.append(&type, sizeof(PacketType));
        packet.append(data, size);
        return packet;
    }
    template <Protocol P>
    static ParsedPacket Parse(sf::Packet packet)
    {
        sf::Uint8 *rawPacketData = (sf::Uint8 *)packet.getData();

        ParsedPacket parsedPacket;
        parsedPacket.fullSize = packet.getDataSize();
        parsedPacket.dataSize = parsedPacket.fullSize - sizeof(PacketType);
        parsedPacket.type = GetTypeFromRawPacket(packet);
        parsedPacket.data = new sf::Uint8[parsedPacket.dataSize];
        memcpy(parsedPacket.data, rawPacketData + sizeof(PacketType), parsedPacket.dataSize);
        return parsedPacket;
    }

    static PacketType GetTypeFromRawPacket(const sf::Packet &rawPacket);
};