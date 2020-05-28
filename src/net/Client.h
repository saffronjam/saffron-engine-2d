#pragma once

#include <mutex>
#include <thread>

#include <SFML/System/Sleep.hpp>

#include "Log.h"
#include "INetMgr.h"
#include "IPacketHandler.h"
#include "PacketMgr.h"
#include "ServerInfo.h"
#include "PingModule.h"
#include "SetupModule.h"
#include "GenericThrowMacros.h"

class Client : public INetMgr, public IPacketHandler
{
public:
    Client(sf::IpAddress address = sf::IpAddress::None, sf::Uint16 port = 0);
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
    void NewUdpConnection(sf::Uint64 uid, const sf::IpAddress address, const sf::Uint16 &port) override;
    std::optional<const Connection *> GetConnectionByUID(sf::Uint64 uid) override;
    std::optional<const IConnInfo *> GetConnInfoByUID(sf::Uint64 uid) override;

private:
    ServerInfo m_serverInfo;
    Connection m_connection;

    sf::IpAddress m_cachedAddress;
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
        INetMgr::Send<P>(type, data, &m_connection);
    }
    else if constexpr (P == Protocol::UDP)
    {
        INetMgr::Send<P>(type, data, &m_connection);
    }
}
template <Protocol P>
void Client::SendEmpty(PacketType type)
{
    if constexpr (P == Protocol::TCP)
    {
        INetMgr::SendEmpty<P>(type, &m_connection);
    }
    else if constexpr (P == Protocol::UDP)
    {
        INetMgr::SendEmpty<P>(type, &m_connection);
    }
}
template <Protocol P, typename T>
void Client::SendArray(PacketType type, const T *data, int nElements)
{
    if constexpr (P == Protocol::TCP)
    {
        INetMgr::SendArray<P>(type, data, nElements, &m_connection);
    }
    else if constexpr (P == Protocol::UDP)
    {
        INetMgr::SendArray<P>(type, data, nElements, &m_connection);
    }
}
template <Protocol P>
void Client::SendRaw(PacketType type, const sf::Uint8 *data, size_t size)
{
    if constexpr (P == Protocol::TCP)
    {
        INetMgr::SendRaw<P>(type, data, size, &m_connection);
    }
    else if constexpr (P == Protocol::UDP)
    {
        INetMgr::SendRaw<P>(type, data, size, &m_connection);
    }
}