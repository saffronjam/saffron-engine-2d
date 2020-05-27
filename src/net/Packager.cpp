#include "Packager.h"

PacketType Packager::GetTypeFromRawPacket(const sf::Packet &rawPacket)
{
    return *(PacketType *)rawPacket.getData();
}