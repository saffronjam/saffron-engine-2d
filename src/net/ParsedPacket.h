#pragma once

#include <vector>

#include <SFML/Config.hpp>

#include "PacketType.h"

struct ParsedPacket
{
    PacketType type = PacketType::None;
    sf::Uint8 *data = nullptr;
    size_t dataSize = 0;
    size_t fullSize = 0;
};

using PacketBuffer = std::vector<ParsedPacket>;