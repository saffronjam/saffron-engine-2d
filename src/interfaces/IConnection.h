#pragma once

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "Protocol.h"

class IConnection
{
public:
    bool operator<(const IConnection &rhs) const
    {
        return true;
    }

    virtual sf::Socket &GetISocket() const noexcept = 0;
    virtual sf::IpAddress GetRemoteAddress() const = 0;
    virtual sf::Uint16 GetRemotePort() const = 0;
    virtual sf::Uint16 GetLocalPort() const = 0;

    virtual void SetRemoteAddress(const sf::IpAddress &address) noexcept = 0;
    virtual void SetRemotePort(const sf::Uint16 &port) noexcept = 0;
    virtual bool IsParent() const noexcept = 0;
};