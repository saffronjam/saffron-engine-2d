#pragma once

#include <SFML/Config.hpp>

enum class Protocol : sf::Uint8
{
    UDP,
    TCP,
    None // Used ONLY as fallback. Attempting to use this will be treated as using undefined protocol
};

template <typename S>
static constexpr Protocol GetProtocolFromSocketType() noexcept
{
    if constexpr (std::is_same<S, sf::TcpListener>::value)
    {
        return Protocol::TCP;
    }
    else if constexpr (std::is_same<S, sf::TcpSocket>::value)
    {
        return Protocol::TCP;
    }
    else if constexpr (std::is_same<S, sf::UdpSocket>::value)
    {
        return Protocol::UDP;
    }
    return Protocol::None;
}