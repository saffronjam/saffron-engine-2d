#pragma once

#include <vector>
#include <map>
#include <set>
#include <mutex>
#include <thread>

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

#include "Packager.h"
#include "IException.h"
#include "GenericThrowMacros.h"
#include "Log.h"

class INetMgr
{
public:
    INetMgr();
    ~INetMgr();

    void StartListening();
    void StopListening();

    template <typename S>
    void AddToSocketSelector(const Connection<S> *conn);
    template <typename S>
    void RemoveFromSocketSelector(const Connection<S> *conn);
    void ClearSocketSelector() noexcept { m_socketSelector.clear(); }

    template <Protocol P, typename T, typename S>
    void Send(PacketType type, const T &data, const Connection<S> &conn);
    template <Protocol P, typename T, typename S>
    void SendArray(PacketType type, const T *data, int nElements, const Connection<S> &conn);
    template <Protocol P, typename S>
    void SendRaw(PacketType type, const sf::Uint8 *data, size_t size, const Connection<S> &conn);

    template <typename S>
    void Receive(const Connection<S> *conn);

    virtual void NewConnection(const Connection<sf::TcpListener> *listener){};

private:
    template <Protocol P, typename S>
    void SendOut(sf::Packet &packet, const Connection<S> &conn);
    void ListenerThreadFn();

protected:
    PacketBuffer m_inBuffer;
    std::mutex m_inBufferLock;

    bool m_listening;

private:
    sf::SocketSelector m_socketSelector;

    std::set<const Connection<sf::TcpListener> *> m_allTcpListenerRefs;
    std::set<const Connection<sf::TcpSocket> *> m_allTcpSocketRefs;
    std::set<const Connection<sf::UdpSocket> *> m_allUdpSocketRefs;
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

template <typename S>
void INetMgr::AddToSocketSelector(const Connection<S> *conn)
{
    if constexpr (std::is_same<S, sf::TcpListener>::value)
    {
        m_socketSelector.add(const_cast<Connection<S> *>(conn)->GetSocket());
        m_allTcpListenerRefs.emplace(conn);
    }
    else if constexpr (std::is_same<S, sf::TcpSocket>::value)
    {
        m_socketSelector.add(const_cast<Connection<S> *>(conn)->GetSocket());
        m_allTcpSocketRefs.emplace(conn);
    }
    else if constexpr (std::is_same<S, sf::UdpSocket>::value)
    {
        m_socketSelector.add(const_cast<Connection<S> *>(conn)->GetSocket());
        m_allUdpSocketRefs.emplace(conn);
    }
    else
    {
        THROW(Exception, "Tried to add a non-socket type to socket selector:  %s", typeid(S).name());
    }
}

template <typename S>
void INetMgr::RemoveFromSocketSelector(const Connection<S> *conn)
{
    if constexpr (std::is_same<S, sf::TcpListener>::value)
    {
        m_socketSelector.remove(const_cast<Connection<S> *>(conn)->GetSocket());
        m_allTcpListenerRefs.erase(conn);
    }
    else if constexpr (std::is_same<S, sf::TcpSocket>::value)
    {
        m_socketSelector.remove(const_cast<Connection<S> *>(conn)->GetSocket());
        m_allTcpSocketRefs.erase(conn);
    }
    else if constexpr (std::is_same<S, sf::UdpSocket>::value)
    {
        m_socketSelector.remove(const_cast<Connection<S> *>(conn)->GetSocket());
        m_allUdpSocketRefs.erase(conn);
    }
    else
    {
        THROW(Exception, "Tried to remove a non-socket type from socket selector:  %s", typeid(S).name());
    }
}
template <Protocol P, typename T, typename S>
void INetMgr::Send(PacketType type, const T &data, const Connection<S> &conn)
{
    sf::Packet outgoing = Packager::Pack(type, reinterpret_cast<const sf::Uint8 *>(&data), sizeof(T));
    SendOut<P>(outgoing, conn);
}
template <Protocol P, typename T, typename S>
void INetMgr::SendArray(PacketType type, const T *data, int nElements, const Connection<S> &conn)
{
    sf::Packet outgoing = Packager::Pack(type, reinterpret_cast<const sf::Uint8 *>(data), sizeof(T) * nElements);
    SendOut<P>(outgoing, conn);
}
template <Protocol P, typename S>
void INetMgr::SendRaw(PacketType type, const sf::Uint8 *data, size_t size, const Connection<S> &conn)
{
    sf::Packet outgoing = Packager::Pack(type, data, size);
    SendOut<P>(outgoing, conn);
}

template <typename S>
void INetMgr::Receive(const Connection<S> *conn)
{
    sf::Packet incoming;
    static constexpr Protocol protocol = GetProtocolFromSocketType<S>();
    if constexpr (protocol == Protocol::TCP)
    {
        conn->GetSocket().receive(incoming);
        m_inBufferLock.lock();
        m_inBuffer.push_back(Packager::Parse<Protocol::TCP>(incoming));
        m_inBufferLock.unlock();
    }
    else if constexpr (protocol == Protocol::UDP)
    {
        sf::IpAddress ip = conn->GetRemoteAddress();
        sf::Uint16 port = conn->GetRemotePort();
        conn->GetSocket().receive(incoming, ip, port);
        m_inBufferLock.lock();
        m_inBuffer.push_back(Packager::Parse<Protocol::TCP>(incoming));
        m_inBufferLock.unlock();
    }
    else
    {
        THROW(Exception, "Tried to receive data on non-socket type:  %s", typeid(S).name());
    }
    return;
}

template <Protocol P, typename S>
void INetMgr::SendOut(sf::Packet &packet, const Connection<S> &conn)
{
    if constexpr (P == Protocol::TCP)
    {
        conn.GetSocket().send(packet);
    }
    else if constexpr (P == Protocol::UDP)
    {
        sf::IpAddress ip = conn.GetRemoteAddress();
        sf::Uint16 port = conn.GetRemotePort();
        conn.GetSocket().send(packet, ip, port);
    }
    else
    {
        THROW(Exception, "Tried to send a packet with invalid protocol:  %u", P);
    }
}