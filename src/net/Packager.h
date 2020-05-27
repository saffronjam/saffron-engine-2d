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
    static sf::Packet Pack(PacketType type, const sf::Uint8 *data, size_t size) noexcept;
    static std::optional<ParsedPacket> Parse(sf::Packet packet) noexcept;

    static PacketType GetTypeFromRawPacket(const sf::Packet &rawPacket) noexcept;
};