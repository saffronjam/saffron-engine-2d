#include "Packager.h"

sf::Packet Packager::Pack(PacketType type, const sf::Uint8 *data, size_t size) noexcept
{
    sf::Packet packet;
    packet.append(&type, sizeof(PacketType));
    if (data)
        packet.append(data, size);
    return packet;
}

PacketType Packager::GetTypeFromRawPacket(const sf::Packet &rawPacket) noexcept
{
    return *(PacketType *)rawPacket.getData();
}