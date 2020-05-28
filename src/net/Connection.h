#pragma once

#include <string>
#include <memory>
#include <optional>
#include <string>
#include <cassert>

#include "Protocol.h"
#include "IConnection.h"
#include "Log.h"

template <typename S>
class Connection : public IConnection
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

    S &GetSocket() const noexcept
    {
        return m_socket.has_value() ? *m_socket.value() : *m_socketRef.value();
    }

    sf::Socket &GetISocket() const noexcept override
    {
        return m_socket.has_value() ? *m_socket.value() : *m_socketRef.value();
    }

    bool IsParent() const noexcept override { return m_socketRef.has_value(); }

    sf::IpAddress GetRemoteAddress() const override
    {
        assert((std::is_same<S, sf::UdpSocket>::value || std::is_same<S, sf::TcpSocket>::value) && (std::string(__PRETTY_FUNCTION__) + "Function is only valid for UDP- and TCP-sockets").c_str());
        if constexpr (std::is_same<S, sf::TcpSocket>::value)
        {
            return m_socket.value()->getRemoteAddress();
        }
        else
        {
            return m_remoteAddress;
        }
    }

    sf::Uint16 GetRemotePort() const override
    {
        assert((std::is_same<S, sf::UdpSocket>::value || std::is_same<S, sf::TcpSocket>::value) && (std::string(__PRETTY_FUNCTION__) + "Function is only valid for UDP- and TCP-sockets").c_str());
        if constexpr (std::is_same<S, sf::TcpSocket>::value)
        {
            return m_socket.value()->getRemotePort();
        }
        else
        {
            return m_remotePort;
        }
    }

    sf::Uint16 GetLocalPort() const override
    {
        assert((std::is_same<S, sf::UdpSocket>::value || std::is_same<S, sf::TcpSocket>::value) && (std::string(__PRETTY_FUNCTION__) + "Function is only valid for UDP- and TCP-sockets").c_str());
        return m_socket.value()->getLocalPort();
    }

    void SetRemoteAddress(const sf::IpAddress &address) noexcept override
    {
        assert((std::is_same<S, sf::UdpSocket>::value) && (std::string(__PRETTY_FUNCTION__) + "Function is only valid for UDP-sockets").c_str());
        m_remoteAddress = address;
    }

    void SetRemotePort(const sf::Uint16 &port) noexcept override
    {
        assert((std::is_same<S, sf::UdpSocket>::value) && (std::string(__PRETTY_FUNCTION__) + "Function is only valid for UDP-sockets").c_str());
        m_remotePort = port;
    }

private:
    std::optional<S *> m_socket;
    std::optional<S *> m_socketRef;
    sf::IpAddress m_remoteAddress;
    sf::Uint16 m_remotePort;
};

template <Protocol>
struct socketReturn
{
    using type = int;
};

template <>
struct socketReturn<Protocol::TCP>
{
    using type = const Connection<sf::TcpSocket> *;
};

template <>
struct socketReturn<Protocol::UDP>
{
    using type = const Connection<sf::UdpSocket> *;
};

template <Protocol P>
typename socketReturn<P>::type ConnCast(const IConnection *conn)
{
    if constexpr (P == Protocol::TCP)
    {
        return dynamic_cast<const Connection<sf::TcpSocket> *>(conn);
    }
    else if constexpr (P == Protocol::TCP)
    {
        return dynamic_cast<const Connection<sf::UdpSocket> *>(conn);
    }
    else
    {
        return 0;
    }
}