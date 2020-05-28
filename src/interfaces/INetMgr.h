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

#include "Protocol.h"
#include "IException.h"
#include "INetModule.h"
#include "IConnInfo.h"
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

    void AddToSocketSelector(const Connection *conn);
    void AddToSocketSelector(sf::TcpListener *listener);
    void RemoveFromSocketSelector(const Connection *conn);
    void RemoveFromSocketSelector(sf::TcpListener *listener);
    void ClearSocketSelector() noexcept { m_socketSelector.clear(); }

    void AddNetModule(std::unique_ptr<INetModule> netModule);

    void ClearInBuffer() { m_inBuffer.clear(); }

    template <Protocol P, typename T>
    void Send(PacketType type, const T &data, const Connection *conn);
    template <Protocol P>
    void SendEmpty(PacketType type, const Connection *conn);
    template <Protocol P, typename T>
    void SendArray(PacketType type, const T *data, int nElements, const Connection *conn);
    template <Protocol P>
    void SendRaw(PacketType type, const sf::Uint8 *data, size_t size, const Connection *conn);
    template <Protocol P>
    void Receive(const Connection *conn);

    void SetUID(sf::Uint64 uid) noexcept { m_uid = uid; }
    sf::Uint64 GetUID() const noexcept { return m_uid; }

protected:
    virtual void NewTcpConnection(sf::TcpListener *listener){};
    virtual void NewUdpConnection(sf::Uint64 uid, const sf::IpAddress address, const sf::Uint16 &port){};
    virtual std::optional<const Connection *> GetConnectionByUID(sf::Uint64 uid) = 0;
    virtual std::optional<const IConnInfo *> GetConnInfoByUID(sf::Uint64 uid) = 0;

private:
    template <Protocol P>
    void SendOut(sf::Packet &packet, const Connection *conn);
    void ListenerThreadFn();

private:
    sf::Uint64 m_uid;

    bool m_listening;
    std::thread m_listenerThread;
    sf::SocketSelector m_socketSelector;

    PacketBuffer m_inBuffer;
    std::mutex m_inBufferLock;

    std::set<std::unique_ptr<INetModule>> m_netModules;
    std::set<sf::TcpListener *> m_tcpListenerRefs;
    std::set<const Connection *> m_connectionRefs;

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
void INetMgr::Send(PacketType type, const T &data, const Connection *conn)
{
    sf::Packet outgoing = Packager::Pack(type, m_uid, reinterpret_cast<const sf::Uint8 *>(&data), sizeof(T));
    SendOut<P>(outgoing, conn);
}
template <Protocol P>
void INetMgr::SendEmpty(PacketType type, const Connection *conn)
{
    sf::Packet outgoing = Packager::Pack(type, m_uid, nullptr, 0);
    SendOut<P>(outgoing, conn);
}
template <Protocol P, typename T>
void INetMgr::SendArray(PacketType type, const T *data, int nElements, const Connection *conn)
{
    sf::Packet outgoing = Packager::Pack(type, m_uid, reinterpret_cast<const sf::Uint8 *>(data), sizeof(T) * nElements);
    SendOut<P>(outgoing, conn);
}
template <Protocol P>
void INetMgr::SendRaw(PacketType type, const sf::Uint8 *data, size_t size, const Connection *conn)
{
    sf::Packet outgoing = Packager::Pack(type, m_uid, data, size);
    SendOut<P>(outgoing, conn);
}

template <Protocol P>
void INetMgr::SendOut(sf::Packet &packet, const Connection *conn)
{
    try
    {
        if constexpr (P == Protocol::TCP)
        {
            sf::Socket::Status status;
            while ((status = conn->GetTcpSocket().send(packet)) != sf::Socket::Done)
            {
                if (status == sf::Socket::Status::Error)
                {
                    auto &socket = conn->GetTcpSocket();
                    THROW(Exception, "Failed to send TCP-packet: %s:%u", socket.getRemoteAddress().toString().c_str(), socket.getRemotePort());
                }
                else if (status == sf::Socket::Status::Disconnected)
                {
                    auto &socket = conn->GetTcpSocket();
                    THROW(Exception, "Failed to send TCP-packet because recipient disconnected: %s:%u", socket.getRemoteAddress().toString().c_str(), socket.getRemotePort());
                }
            }
        }
        else if constexpr (P == Protocol::UDP)
        {
            sf::IpAddress address = conn->GetUdpRemoteAddress();
            sf::Uint16 port = conn->GetUdpRemotePort();
            sf::Socket::Status status;
            while ((status = conn->GetUdpSocket().send(packet, address, port)) != sf::Socket::Done)
            {
                if (status == sf::Socket::Status::Error)
                {
                    THROW(Exception, "Failed to send UDP-packet: %s:%u", address.toString().c_str(), port);
                }
                else if (status == sf::Socket::Status::Disconnected)
                {
                    THROW(Exception, "Failed to send UDP-packet because recipient disconnected: %s:%u", address.toString().c_str(), port);
                }
            }
        }
        else
        {
            THROW(Exception, "Tried to send a packet with invalid protocol: %u", P);
        }
    }
    LogOnly;
}

template <Protocol P>
void INetMgr::Receive(const Connection *conn)
{
    try
    {
        sf::Packet incoming;
        std::optional<const IConnInfo *> connInfoOpt = std::nullopt;
        std::optional<sf::Uint64> uidOpt = std::nullopt;

        if constexpr (P == Protocol::TCP)
        {
            auto &socket = conn->GetTcpSocket();
            sf::Socket::Status status;
            while ((status = socket.receive(incoming)) != sf::Socket::Done)
            {
                if (status == sf::Socket::Error)
                {
                    THROW(Exception, "Failed to receive TCP-packet: %s:%u", socket.getRemoteAddress().toString().c_str(), socket.getRemotePort());
                }
                else if (status == sf::Socket::Disconnected)
                {
                    // HandleClosedConnection()
                    return;
                }
            }
            if (incoming.getDataSize() < sizeof(PacketType))
                THROW(Exception, "Bad data size in TCP-packet: %s:%u", socket.getRemoteAddress().toString().c_str(), socket.getRemotePort());

            // Make sure an UID is present in the packet
            if (!(uidOpt = Packager::GetUIDFromRawPacket(incoming)).has_value())
                THROW(Exception, "UID was not found in TCP-packet: %s:%u", socket.getRemoteAddress().toString().c_str(), socket.getRemotePort());
            // Make sure the UID is valid
            if (uidOpt.value() == 0)
                THROW(Exception, "UID was 0 in TCP-packet: %s:%u", socket.getRemoteAddress().toString().c_str(), socket.getRemotePort());
            // Make sure connInfo is available
            if (!(connInfoOpt = GetConnInfoByUID(uidOpt.value())).has_value())
                THROW(Exception, "No ConnInfo was found for connection behind TCP-packet: %s:%u", socket.getRemoteAddress().toString().c_str(), socket.getRemotePort());
        }
        else if constexpr (P == Protocol::UDP)
        {
            auto &socket = conn->GetUdpSocket();
            sf::IpAddress address;
            sf::Uint16 port;
            sf::Socket::Status status;
            while ((status = socket.receive(incoming, address, port)) != sf::Socket::Done)
            {
                if (status == sf::Socket::Error)
                    THROW(Exception, "Failed to receive UDP-packet: %s:%u", conn->GetUdpRemoteAddress().toString().c_str(), conn->GetUdpRemotePort());
            }

            if (incoming.getDataSize() < sizeof(PacketType))
                THROW(Exception, "Bad data size in UDP-packet: %s:%u", conn->GetUdpRemoteAddress().toString().c_str(), conn->GetUdpRemotePort());

            // Make sure an UID is present in the packet
            if (!(uidOpt = Packager::GetUIDFromRawPacket(incoming)).has_value())
                THROW(Exception, "UID was not found in UDP-packet: %s:%u", conn->GetUdpRemoteAddress().toString().c_str(), conn->GetUdpRemotePort());
            // Make sure the UID is valid
            if (uidOpt.value() == 0)
                THROW(Exception, "UID was 0 in UDP-packet: %s:%u", conn->GetUdpRemoteAddress().toString().c_str(), conn->GetUdpRemotePort());
            // Make sure connInfo is available
            if (!(connInfoOpt = GetConnInfoByUID(uidOpt.value())).has_value())
                THROW(Exception, "No ConnInfo was found for connection behind UDP-packet: %s:%u", conn->GetUdpRemoteAddress().toString().c_str(), conn->GetUdpRemotePort());

            NewUdpConnection(uidOpt.value(), address, port);
        }
        else
            THROW(Exception, "Failed cast connection type to TCP och UDP: %s:%u", conn->GetUdpRemoteAddress().toString().c_str(), conn->GetUdpRemotePort());

        std::optional<ParsedPacket> parseAttempt;
        parseAttempt = Packager::Parse<P>(incoming, conn, connInfoOpt.value());
        if (parseAttempt.has_value())
        {
            m_inBufferLock.lock();
            m_inBuffer.push_back(parseAttempt.value());
            m_inBufferLock.unlock();
        }
    }
    LogOnly;
}