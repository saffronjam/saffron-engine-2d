#include "Client.h"

Client::Client(sf::IpAddress address, sf::Uint16 port)
    : m_serverInfo(1),
      m_cachedAddress(address),
      m_cachedPort(port),
      m_connState(ConnState::Disconnected),
      m_autoReconnect(false)
{
    PacketMgr::AddHandler(this);
    AddNetModule(std::make_unique<PingModule>(this));
    AddNetModule(std::make_unique<SetupModule>(this));
    StartListening();
}

Client::~Client()
{
    StopListening();
    if (m_connState != ConnState::Disconnected)
        Disconnect();
}

void Client::Connect()
{
    if (m_connState == ConnState::Connected)
    {
        THROW(Exception, "Tried to connect an already connected client: %s", "Try to disconnect before connecting");
    }
    if (m_cachedAddress == sf::IpAddress::None || m_cachedPort == 0)
    {
        THROW(Exception, "Tried to connect a non-configured client: %s", "IP-address and port must be configured with SetNet()");
    }

    m_connState == ConnState::Disconnected;
    m_tryConnectDelay = sf::seconds(0.0f);
    if (m_connector.joinable())
        m_connector.join();

    m_connector = std::thread(&Client::ConnectThreadFn, this);
}

void Client::Disconnect()
{
    if (m_connState == ConnState::Disconnected)
    {
        THROW(Exception, "Tried to disconnect an already disconnected client: %s", "Try to connect before disconnecting");
    }

    m_connection.GetTcpSocket().disconnect();
    m_connection.GetUdpSocket().unbind();
    m_connState = ConnState::Disconnected;
    m_tryConnectDelay = sf::seconds(0.0f);
    if (m_connector.joinable())
        m_connector.join();

    ClearSocketSelector();
    ClearInBuffer();
}

void Client::SetNet(sf::IpAddress address, sf::Uint16 port) noexcept
{
    m_cachedAddress = address;
    m_cachedPort = port;
}

void Client::ConnectThreadFn()
{
    m_tryConnectDelay = sf::seconds(2.0f);
    m_connState = ConnState::TryingToConnect;
    for (; m_connState == ConnState::TryingToConnect; sf::sleep(m_tryConnectDelay))
    {
        try
        {
            if (!m_connectMutex.try_lock())
                return;
            if (m_connection.GetTcpSocket().connect(m_cachedAddress, m_cachedPort) != sf::Socket::Status::Done)
            {
                THROW(Exception, "Could not connect TCP-socket to remote host: %s:%u", m_cachedAddress.toString().c_str(), m_cachedPort);
            }
            if (m_connection.GetUdpSocket().bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
            {
                THROW(Exception, "Could not bind UDP-socket. Port: %s", "Any available port");
            }
            m_connection.SetUdpRemoteAddress(m_cachedAddress);
            m_connection.SetUdpRemotePort(m_cachedPort);

            AddToSocketSelector<TCP>(&m_connection);
            AddToSocketSelector<UDP>(&m_connection);
            m_connState = ConnState::Connected;
            m_tryConnectDelay = sf::seconds(0.0f);
            m_serverInfo.Reset();
            m_connectMutex.unlock();
        }
        catch (const IException &e)
        {
#ifdef LOG_EXCEPTION
            LogWhat;
#endif
            m_connection.GetTcpSocket().disconnect();
            m_connection.GetUdpSocket().unbind();
            m_connectMutex.unlock();
        }
        catch (const std::exception &e)
        {
#ifdef LOG_EXCEPTION
            LogWhat;
#endif
            m_connection.GetTcpSocket().disconnect();
            m_connection.GetUdpSocket().unbind();
            m_connectMutex.unlock();
        }
        catch (...)
        {
#ifdef LOG_EXCEPTION
            LogNoDetails;
#endif
            m_connection.GetTcpSocket().disconnect();
            m_connection.GetUdpSocket().unbind();
            m_connectMutex.unlock();
        }
    };
}

void Client::NewUdpConnection(sf::Uint64 uid, const sf::IpAddress address, const sf::Uint16 &port)
{
    if (uid == 1)
    {
        m_connection.SetUdpRemoteAddress(address);
        m_connection.SetUdpRemotePort(port);
    }
}

void Client::HandleClosedConnection(const Connection *conn)
{
    if (&m_connection == conn)
    {
        Disconnect();
        if (m_autoReconnect)
            Connect();
    }
}

std::optional<const Connection *> Client::GetConnectionByUID(sf::Uint64 uid)
{
    if (uid == 1)
        return &m_connection;
    else
        return std::nullopt;
}

std::optional<const IConnInfo *> Client::GetConnInfoByUID(sf::Uint64 uid)
{
    if (uid == 1)
        return &m_serverInfo;
    else
        return std::nullopt;
}