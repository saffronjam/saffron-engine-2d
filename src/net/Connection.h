#pragma once

#include <string>
#include <memory>
#include <optional>

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "Protocol.h"

template <typename S>
class Connection
{
public:
    Connection() noexcept
        : m_socket(new S),
          m_remoteAddress(sf::IpAddress::None),
          m_remotePort(0)
    {
    }
    Connection(sf::Socket *ref) noexcept
        : m_socketRef(ref),
          m_remoteAddress(sf::IpAddress::None),
          m_remotePort(0)
    {
    }
    bool operator<(const Connection &rhs) const
    {
        return true;
    }

    S &GetSocket() const noexcept
    {
        return m_socket.has_value() ? *m_socket.value() : *m_socketRef.value();
    }

    bool IsParent() const noexcept { return m_socketRef.has_value(); }

    template <class Q = S>
    typename std::enable_if<std::is_same<Q, sf::TcpSocket>::value || std::is_same<Q, sf::UdpSocket>::value, sf::IpAddress>::type GetRemoteAddress() const
    {
        if constexpr (std::is_same<S, sf::TcpSocket>::value)
        {
            return m_socket->getRemoteAddress();
        }
        else if constexpr (std::is_same<S, sf::UdpSocket>::value)
        {
            return m_remoteAddress;
        }
        return sf::IpAddress::None;
    }

    template <class Q = S>
    typename std::enable_if<std::is_same<Q, sf::TcpSocket>::value || std::is_same<Q, sf::UdpSocket>::value, sf::Uint16>::type GetRemotePort() const
    {
        if constexpr (std::is_same<S, sf::TcpSocket>::value)
        {
            return m_socket->getRemotePort();
        }
        else if constexpr (std::is_same<S, sf::UdpSocket>::value)
        {
            return m_remotePort;
        }
        return 0u;
    }

    template <class Q = S>
    typename std::enable_if<std::is_same<Q, sf::UdpSocket>::value, void>::type SetRemoteAddress(const sf::IpAddress &ip) noexcept
    {
        m_remoteAddress = ip;
    }
    template <class Q = S>
    typename std::enable_if<std::is_same<Q, sf::UdpSocket>::value, void>::type SetRemotePort(const sf::Uint16 &port) noexcept
    {
        m_remotePort = port;
    }

private:
    std::optional<S *> m_socket;
    std::optional<S *> m_socketRef;
    sf::IpAddress m_remoteAddress;
    sf::Uint16 m_remotePort;
};
