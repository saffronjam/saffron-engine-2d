#pragma once

#include <vector>
#include <map>
#include <set>
#include <mutex>
#include <thread>
#include <memory>

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "IException.h"
#include "INetModule.h"
#include "Packager.h"
#include "PacketMgr.h"
#include "GenericThrowMacros.h"
#include "Log.h"

class INetMgr
{
public:
    INetMgr();
    ~INetMgr();

    void StartListening();
    void StopListening();

    void AddToSocketSelector(const IConnection *iconn);
    void RemoveFromSocketSelector(const IConnection *iconn);
    void ClearSocketSelector() noexcept { m_socketSelector.clear(); }

    void AddNetModule(std::unique_ptr<INetModule> netModule);

    template <Protocol P, typename T>
    void Send(PacketType type, const T &data, const IConnection *iconn);
    template <Protocol P>
    void SendEmpty(PacketType type, const IConnection *iconn);
    template <Protocol P, typename T>
    void SendArray(PacketType type, const T *data, int nElements, const IConnection *iconn);
    template <Protocol P>
    void SendRaw(PacketType type, const sf::Uint8 *data, size_t size, const IConnection *iconn);

    void Receive(const IConnection *conn);

    virtual void NewConnection(const Connection<sf::TcpListener> *listener){};

private:
    template <Protocol P>
    void SendOut(sf::Packet &packet, const IConnection *iconn);
    void ListenerThreadFn();

protected:
    PacketBuffer m_inBuffer;
    std::mutex m_inBufferLock;

    bool m_listening;

private:
    sf::SocketSelector m_socketSelector;

    std::set<std::unique_ptr<INetModule>> m_netModules;
    std::set<const IConnection *> m_connectionRefs;
    std::thread m_listenerThread;

public:
    class Exception : public IException
    {
    public:
        Exception(int line, const char *file, const char *errorString) noexcept;
        const char *what() const noexcept override;
        virtual const char *GetType() const noexcept override;
        const char *GetErrorString() const noexcept;

    private:
        std::string errorString;
    };
};

template <Protocol P, typename T>
void INetMgr::Send(PacketType type, const T &data, const IConnection *iconn)
{
    sf::Packet outgoing = Packager::Pack(type, reinterpret_cast<const sf::Uint8 *>(&data), sizeof(T));
    SendOut<P>(outgoing, iconn);
}
template <Protocol P>
void INetMgr::SendEmpty(PacketType type, const IConnection *iconn)
{
    sf::Packet outgoing = Packager::Pack(type, nullptr, 0);
    SendOut<P>(outgoing, iconn);
}
template <Protocol P, typename T>
void INetMgr::SendArray(PacketType type, const T *data, int nElements, const IConnection *iconn)
{
    sf::Packet outgoing = Packager::Pack(type, reinterpret_cast<const sf::Uint8 *>(data), sizeof(T) * nElements);
    SendOut<P>(outgoing, iconn);
}
template <Protocol P>
void INetMgr::SendRaw(PacketType type, const sf::Uint8 *data, size_t size, const IConnection *iconn)
{
    sf::Packet outgoing = Packager::Pack(type, data, size);
    SendOut<P>(outgoing, iconn);
}

template <Protocol P>
void INetMgr::SendOut(sf::Packet &packet, const IConnection *iconn)
{
    if constexpr (P == Protocol::TCP)
    {
        const Connection<sf::TcpSocket> *tcpConn = dynamic_cast<const Connection<sf::TcpSocket> *>(iconn);
        if (!tcpConn)
            return;
        tcpConn->GetSocket().send(packet);
    }
    else if constexpr (P == Protocol::UDP)
    {
        const Connection<sf::UdpSocket> *udpConn = dynamic_cast<const Connection<sf::UdpSocket> *>(iconn);
        if (!udpConn)
            return;
        sf::IpAddress ip = iconn->GetRemoteAddress();
        sf::Uint16 port = iconn->GetRemotePort();
        udpConn->GetSocket().send(packet, ip, port);
    }
    else
    {
        THROW(Exception, "Tried to send a packet with invalid protocol:  %u", P);
    }
}