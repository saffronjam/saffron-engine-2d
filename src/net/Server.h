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
    template <Protocol P, typename T>
    void BroadcastArray(PacketType type, const T *data, int nElements);
    template <Protocol P>
    void BroadcastRaw(PacketType type, const sf::Uint8 *data, size_t size);

private:
    void OpenThreadFn();
    void NewConnection(const Connection<sf::TcpListener> *listener) override;

private:
    Connection<sf::TcpListener> m_tcpListener;
    std::map<Connection<sf::TcpSocket>, std::set<ClientInfo>::iterator> m_clientTcpConnections;
    std::map<Connection<sf::UdpSocket>, std::set<ClientInfo>::iterator> m_clientUdpConnections;
    Connection<sf::UdpSocket> m_udpConnection;
    std::set<ClientInfo> m_clientInfoSet;
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
};

template <Protocol P, typename T>
void Server::Broadcast(PacketType type, const T &data)
{
    if constexpr (P == Protocol::TCP)
    {
        for (auto &conn : m_clientTcpConnections)
            INetMgr::Send<P>(type, data, conn);
    }
    else if constexpr (P == Protocol::UDP)
    {
        for (auto &conn : m_clientUdpConnections)
            INetMgr::Send<P>(type, data, conn);
    }
}
template <Protocol P, typename T>
void Server::BroadcastArray(PacketType type, const T *data, int nElements)
{
    if constexpr (P == Protocol::TCP)
    {
        for (auto &conn : m_clientTcpConnections)
            INetMgr::SendArray<P>(type, data, nElements, conn);
    }
    else if constexpr (P == Protocol::UDP)
    {
        for (auto &conn : m_clientUdpConnections)
            INetMgr::SendArray<P>(type, data, nElements, conn);
    }
}
template <Protocol P>
void Server::BroadcastRaw(PacketType type, const sf::Uint8 *data, size_t size)
{
    if constexpr (P == Protocol::TCP)
    {
        for (auto &conn : m_clientTcpConnections)
            INetMgr::SendRaw<P>(type, data, size, conn);
    }
    else if constexpr (P == Protocol::UDP)
    {
        for (auto &conn : m_clientUdpConnections)
            INetMgr::SendRaw<P>(type, data, size, conn);
    }
}