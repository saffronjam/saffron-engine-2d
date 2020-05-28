#pragma once

#include <mutex>
#include <thread>
#include <set>

#include <SFML/System/Sleep.hpp>

#include "INetMgr.h"
#include "IPacketHandler.h"
#include "ClientInfo.h"
#include "GenericThrowMacros.h"
#include "Log.h"
#include "PingModule.h"

class Server : public INetMgr, public IPacketHandler
{
public:
    Server(sf::Uint16 port = 0);
    ~Server();
    Server(const Server &) = delete;
    const Server &operator()(const Server &) = delete;

    void Open();
    void Close();

    void SetPort(sf::Uint16 port) noexcept { m_port = port; }

    template <Protocol P, typename T>
    void Broadcast(PacketType type, const T &data);
    template <Protocol P>
    void BroadcastEmpty(PacketType type);
    template <Protocol P, typename T>
    void BroadcastArray(PacketType type, const T *data, int nElements);
    template <Protocol P>
    void BroadcastRaw(PacketType type, const sf::Uint8 *data, size_t size);

private:
    void OpenThreadFn();
    void NewTcpConnection(sf::TcpListener *listener) override;
    void NewUdpConnection(sf::Uint64 uid, const sf::IpAddress address, const sf::Uint16 &port) override;
    std::optional<const Connection *> GetConnectionByUID(sf::Uint64 uid) override;
    std::optional<const IConnInfo *> GetConnInfoByUID(sf::Uint64 uid) override;

    sf::Uint64 GenerateUID() noexcept;

private:
    sf::TcpListener m_tcpListener;
    std::map<Connection, ClientInfo> m_clientConnections;
    Connection m_udpConnection;
    sf::Uint16 m_port;

    std::thread m_opener;
    std::mutex m_openMutex;
    sf::Time m_tryOpenDelay;
    enum class ConnState
    {
        Opened,
        Closed,
        TryingToOpen
    } m_connState;

    sf::Uint64 m_nextUID;
};

template <Protocol P, typename T>
void Server::Broadcast(PacketType type, const T &data)
{
    for (auto &[conn, info] : m_clientConnections)
        INetMgr::Send<P>(type, data, conn);
}
template <Protocol P>
void Server::BroadcastEmpty(PacketType type)
{
    for (auto &[conn, info] : m_clientConnections)
        INetMgr::SendEmpty<P>(type, &conn);
}
template <Protocol P, typename T>
void Server::BroadcastArray(PacketType type, const T *data, int nElements)
{
    for (auto &[conn, info] : m_clientConnections)
        INetMgr::SendArray<P>(type, data, nElements, &conn);
}
template <Protocol P>
void Server::BroadcastRaw(PacketType type, const sf::Uint8 *data, size_t size)
{
    for (auto &[conn, info] : m_clientConnections)
        INetMgr::SendRaw<P>(type, data, size, &conn);
}