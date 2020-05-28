#include "Server.h"

Server::Server(sf::Uint16 port)
    : m_port(port),
      m_connState(ConnState::Closed)
{
    PacketMgr::AddHandler(this);
    PacketMgr::AddPacketBuffer(&m_inBuffer, &m_inBufferLock);
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
    m_tryOpenDelay = sf::seconds(0.0f);
    m_tcpListener.GetSocket().close();
    m_udpConnection.GetSocket().unbind();
    for (auto &[conn, info] : m_clientTcpConnections)
    {
        if (!conn.IsParent())
            const_cast<Connection<sf::TcpSocket> &>(conn).GetSocket().disconnect();
    }
    for (auto &[conn, info] : m_clientUdpConnections)
    {
        if (!conn.IsParent())
            const_cast<Connection<sf::UdpSocket> &>(conn).GetSocket().unbind();
    }
    ClearSocketSelector();
    if (m_opener.joinable())
        m_opener.join();

    StopListening();
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
            if (m_tcpListener.GetSocket().listen(m_port) != sf::Socket::Status::Done)
            {
                THROW(Exception, "Failed to start listener on server. Port: %u", m_port);
            }
            if (m_udpConnection.GetSocket().bind(m_port) != sf::Socket::Status::Done)
            {
                THROW(Exception, "Could not bind UDP-socket. Port: %u", m_port);
            }
            AddToSocketSelector(&m_tcpListener);
            AddToSocketSelector(&m_udpConnection);
            StartListening();
            m_connState = ConnState::Opened;
            m_tryOpenDelay = sf::seconds(0.0f);
            m_openMutex.unlock();
        }
        catch (const IException &e)
        {
            m_tcpListener.GetSocket().close();
            m_udpConnection.GetSocket().unbind();
            m_openMutex.unlock();
#ifdef LOG_EXCEPTION
            log_fatal("\n%s", e.what());
#endif
        }
        catch (const std::exception &e)
        {
#ifdef LOG_EXCEPTION
            log_fatal("\n%s", e.what());
#endif
        }
        catch (...)
        {
#ifdef LOG_EXCEPTION
            log_fatal("\n%s", "No details available");
#endif
        }
    };
}

void Server::NewConnection(const Connection<sf::TcpListener> *listener)
{
    Connection<sf::TcpSocket> newConn;
    if (listener->GetSocket().accept(newConn.GetSocket()) != sf::Socket::Status::Done)
        return;

    auto clientInfoRes = m_clientInfoSet.emplace();
    auto clientConnRes = m_clientTcpConnections.emplace(std::make_pair(newConn, clientInfoRes.first));

    AddToSocketSelector(&clientConnRes.first->first);
}
