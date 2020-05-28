#pragma once

#include <mutex>
#include <thread>

#include <SFML/System/Sleep.hpp>

#include "Log.h"
#include "INetMgr.h"
#include "IPacketHandler.h"
#include "PacketMgr.h"
#include "GenericThrowMacros.h"

class Client : public INetMgr, public IPacketHandler
{
public:
    Client(sf::IpAddress ip = sf::IpAddress::None, sf::Uint16 port = 0) noexcept;
    ~Client();
    Client(const Client &) = delete;
    const Client &operator()(const Client &) = delete;

    void Connect();
    void Disconnect();

    void SetNet(sf::IpAddress address, sf::Uint16 port) noexcept;

    template <Protocol P, typename T>
    void Send(PacketType type, const T &data);
    template <Protocol P>
    void SendEmpty(PacketType type);
    template <Protocol P, typename T>
    void SendArray(PacketType type, const T *data, int nElements);
    template <Protocol P>
    void SendRaw(PacketType type, const sf::Uint8 *data, size_t size);

private:
    void ConnectThreadFn();

private:
    Connection<sf::TcpSocket> m_tcpConnection;
    Connection<sf::UdpSocket> m_udpConnection;

    sf::IpAddress m_cachedIP;
    sf::Uint16 m_cachedPort;

    std::thread m_connector;
    std::mutex m_connectMutex;
    sf::Time m_tryConnectDelay;
    enum class ConnState
    {
        Disconnected,
        Connected,
        TryingToConnect
    } m_connState;
};

template <Protocol P, typename T>
void Client::Send(PacketType type, const T &data)
{
    if constexpr (P == Protocol::TCP)
    {
        INetMgr::Send<P>(type, data, &m_tcpConnection);
    }
    else if constexpr (P == Protocol::UDP)
    {
        INetMgr::Send<P>(type, data, &m_udpConnection);
    }
}
template <Protocol P>
void Client::SendEmpty(PacketType type)
{
    if constexpr (P == Protocol::TCP)
    {
        INetMgr::SendEmpty<P>(type, &m_tcpConnection);
    }
    else if constexpr (P == Protocol::UDP)
    {
        INetMgr::SendEmpty<P>(type, &m_udpConnection);
    }
}
template <Protocol P, typename T>
void Client::SendArray(PacketType type, const T *data, int nElements)
{
    if constexpr (P == Protocol::TCP)
    {
        INetMgr::SendArray<P>(type, data, nElements, &m_tcpConnection);
    }
    else if constexpr (P == Protocol::UDP)
    {
        INetMgr::SendArray<P>(type, data, nElements, &m_udpConnection);
    }
}
template <Protocol P>
void Client::SendRaw(PacketType type, const sf::Uint8 *data, size_t size)
{
    if constexpr (P == Protocol::TCP)
    {
        INetMgr::SendRaw<P>(type, data, size, &m_tcpConnection);
    }
    else if constexpr (P == Protocol::UDP)
    {
        INetMgr::SendRaw<P>(type, data, size, &m_udpConnection);
    }
}