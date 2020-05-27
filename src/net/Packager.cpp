#include "Packager.h"

sf::Packet Packager::Pack(PacketType type, const sf::Uint8 *data, size_t size) noexcept
{
    sf::Packet packet;
    packet.append(&type, sizeof(PacketType));
    packet.append(data, size);
    return packet;
}

std::optional<ParsedPacket> Packager::Parse(sf::Packet packet) noexcept
{
    sf::Uint8 *rawPacketData = (sf::Uint8 *)packet.getData();

    ParsedPacket parsedPacket;
    parsedPacket.fullSize = packet.getDataSize();
    parsedPacket.dataSize = parsedPacket.fullSize - sizeof(PacketType);
    parsedPacket.type = GetTypeFromRawPacket(packet);
    parsedPacket.data = new sf::Uint8[parsedPacket.dataSize];
    memcpy(parsedPacket.data, rawPacketData + sizeof(PacketType), parsedPacket.dataSize);

    if (parsedPacket.type > PacketType::Invalid && parsedPacket.type < PacketType::Count)
        return parsedPacket;
    else
        return std::nullopt;
}

PacketType Packager::GetTypeFromRawPacket(const sf::Packet &rawPacket) noexcept
{
    return *(PacketType *)rawPacket.getData();
}