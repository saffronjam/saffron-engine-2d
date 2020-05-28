#pragma once

#include <vector>
#include <memory>

#include <SFML/Config.hpp>

#include "PacketType.h"
#include "IConnection.h"

struct ParsedPacket
{
    PacketType type = PacketType::Invalid;
    std::shared_ptr<sf::Uint8> data;
    size_t dataSize = 0;
    size_t fullSize = 0;
    const IConnection *connection = nullptr;
    Protocol protocol = Protocol::None;
};

using PacketBuffer = std::vector<ParsedPacket>;