#include "Server.h"

Server::Server(sf::Uint16 port)
    : m_port(port),
      m_connState(ConnState::Closed),
      m_nextUID(1) // Start on UID = 1, since 0 is invalid
{
    SetUID(1); // Server has a static UID of 1
    PacketMgr::AddHandler(this);
    AddNetModule(std::unique_ptr<PingModule>(new PingModule(this)));
}

Server::~Server()
{
    if (m_connState != ConnState::Closed)
        Close();
}

void Server::Open()
{
    if (m_connState == ConnState::Opened)
    {
        THROW(Exception, "Tried to open an already opened server: %s", "Try to close before opening the server");
    }
    if (m_port == 0)
    {
        THROW(Exception, "Tried to open a non-configured server. Port: %u", m_port);
    }

    m_connState == ConnState::Closed;
    m_tryOpenDelay = sf::seconds(0.0f);
    if (m_opener.joinable())
        m_opener.join();

    m_tryOpenDelay = sf::seconds(2.0f);
    m_opener = std::thread(&Server::OpenThreadFn, this);
}

void Server::Close()
{
    if (m_connState == ConnState::Closed)
    {
        THROW(Exception, "Tried to close an already closed server: %s", "Try to open before closing the server");
    }
    m_connState = ConnState::Closed;
    m_nextUID = 1u;
    m_tryOpenDelay = sf::seconds(0.0f);
    m_tcpListener.close();

    StopListening();
    ClearSocketSelector();

    for (auto &[conn, info] : m_clientConnections)
    {
        const_cast<Connection &>(conn).GetTcpSocket().disconnect();
        if (!conn.IsUdpParent())
            const_cast<Connection &>(conn).GetUdpSocket().unbind();
    }
    m_udpConnection.GetUdpSocket().unbind();
    m_clientConnections.clear();
    if (m_opener.joinable())
        m_opener.join();

    ClearInBuffer();
}

void Server::OpenThreadFn()
{
    m_connState = ConnState::TryingToOpen;
    for (; m_connState == ConnState::TryingToOpen; sf::sleep(m_tryOpenDelay))
    {
        try
        {
            if (!m_openMutex.try_lock())
                return;
            if (m_tcpListener.listen(m_port) != sf::Socket::Status::Done)
            {
                THROW(Exception, "Failed to start listener on server. Port: %u", m_port);
            }
            if (m_udpConnection.GetUdpSocket().bind(m_port) != sf::Socket::Status::Done)
            {
                THROW(Exception, "Could not bind UDP-socket. Port: %u", m_port);
            }
            StartListening();
            AddToSocketSelector(&m_tcpListener);
            AddToSocketSelector<UDP>(&m_udpConnection);
            m_connState = ConnState::Opened;
            m_tryOpenDelay = sf::seconds(0.0f);
            m_openMutex.unlock();
        }
        catch (const IException &e)
        {
#ifdef LOG_EXCEPTION
            log_fatal("\n%s", e.what());
#endif
            m_tcpListener.close();
            m_udpConnection.GetUdpSocket().unbind();
            m_openMutex.unlock();
        }
        catch (const std::exception &e)
        {
#ifdef LOG_EXCEPTION
            log_fatal("\n%s", e.what());
#endif
            m_tcpListener.close();
            m_udpConnection.GetUdpSocket().unbind();
            m_openMutex.unlock();
        }
        catch (...)
        {
#ifdef LOG_EXCEPTION
            log_fatal("\n%s", "No details available");
#endif
            m_tcpListener.close();
            m_udpConnection.GetUdpSocket().unbind();
            m_openMutex.unlock();
        }
    };
}

void Server::NewTcpConnection(sf::TcpListener *listener)
{
    // Every client is parent over server udp-socket
    Connection newConn(&m_udpConnection.GetUdpSocket());
    if (listener->accept(newConn.GetTcpSocket()) != sf::Socket::Status::Done)
        return;

    sf::Uint64 uid = GenerateUID();

    auto clientConnRes = m_clientConnections.emplace(std::make_pair(newConn, ClientInfo(uid)));

    AddToSocketSelector<TCP>(&clientConnRes.first->first);

    // Giving out UID to connected client
    Send<TCP>(UID, uid, &newConn);
}

void Server::NewUdpConnection(sf::Uint64 uid, const sf::IpAddress address, const sf::Uint16 &port)
{
    try
    {
        auto connOpt = GetConnectionByUID(uid);
        if (!connOpt.has_value())
            THROW(Exception, "UDP address update contained UID that did not correspond to any connection. UID: %u", uid);

        // Update the remote address
        Connection *conn = const_cast<Connection *>(connOpt.value());
        conn->SetUdpRemoteAddress(address);
        conn->SetUdpRemotePort(port);
    }
    LogOnly;
}

void Server::HandleClosedConnection(const Connection *conn)
{
    RemoveFromSocketSelector(conn);
    RemoveClient(conn);
}

void Server::RemoveClient(const Connection *conn)
{
    conn->GetTcpSocket().disconnect();
    if (!conn->IsUdpParent())
        conn->GetUdpSocket().unbind();
    m_clientConnections.erase(*conn);
}

sf::Uint64 Server::GenerateUID() noexcept
{
    return m_nextUID++;
}

std::optional<const Connection *> Server::GetConnectionByUID(sf::Uint64 uid)
{
    // Find connection by ID
    auto connIter = std::find_if(m_clientConnections.begin(), m_clientConnections.end(), [&uid](const std::pair<Connection, ClientInfo> &pair) {
        return pair.second.GetUID() == uid;
    });

    return connIter == m_clientConnections.end() ? std::nullopt : std::optional<const Connection *>(&connIter->first);
}

std::optional<const IConnInfo *> Server::GetConnInfoByUID(sf::Uint64 uid)
{
    auto clientOpt = GetConnectionByUID(uid);

    if (!clientOpt.has_value())
        return std::nullopt;

    Connection conn = *clientOpt.value();
    Connection conn2 = m_clientConnections.begin()->first;

    auto clientInfoAttempt = m_clientConnections.find(conn);
    if (clientInfoAttempt == m_clientConnections.end())
        return std::nullopt;
    else
        return &clientInfoAttempt->second;
}