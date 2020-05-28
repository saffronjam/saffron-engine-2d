#include "Client.h"

Client::Client(sf::IpAddress ip, sf::Uint16 port) noexcept
    : m_connState(ConnState::Disconnected)
{
    PacketMgr::AddHandler(this);
    PacketMgr::AddPacketBuffer(&m_inBuffer, &m_inBufferLock);
    AddNetModule(std::make_unique<PingModule>(this));
}

Client::~Client()
{
    if (m_connState != ConnState::Disconnected)
        Disconnect();
}

void Client::Connect()
{
    if (m_connState == ConnState::Connected)
    {
        THROW(Exception, "Tried to connect an already connected client: %s", "Try to disconnect before connecting");
    }
    if (m_cachedIP == sf::IpAddress::None || m_cachedPort == 0)
    {
        THROW(Exception, "Tried to connect a non-configured client: %s", "IP and port must be configured with SetNet()");
    }

    m_connState == ConnState::Disconnected;
    m_tryConnectDelay = sf::seconds(0.0f);
    if (m_connector.joinable())
        m_connector.join();

    m_tryConnectDelay = sf::seconds(2.0f);
    m_connector = std::thread(&Client::ConnectThreadFn, this);
}

void Client::Disconnect()
{
    if (m_connState == ConnState::Disconnected)
    {
        THROW(Exception, "Tried to disconnect an already disconnected client: %s", "Try to connect before disconnecting");
    }
    m_connState = ConnState::Disconnected;
    m_tryConnectDelay = sf::seconds(0.0f);
    m_tcpConnection.GetSocket().disconnect();
    m_udpConnection.GetSocket().unbind();
    ClearSocketSelector();
    if (m_connector.joinable())
        m_connector.join();
    StopListening();
}

void Client::SetNet(sf::IpAddress ip, sf::Uint16 port) noexcept
{
    m_cachedIP = ip;
    m_cachedPort = port;
}

void Client::ConnectThreadFn()
{
    m_connState = ConnState::TryingToConnect;
    for (; m_connState == ConnState::TryingToConnect; sf::sleep(m_tryConnectDelay))
    {
        try
        {
            if (!m_connectMutex.try_lock())
                return;
            if (m_tcpConnection.GetSocket().connect(m_cachedIP, m_cachedPort) != sf::Socket::Status::Done)
            {
                THROW(Exception, "Could not connect TCP-socket to remote host: %s:%u", m_cachedIP.toString().c_str(), m_cachedPort);
            }
            if (m_udpConnection.GetSocket().bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
            {
                THROW(Exception, "Could not bind UDP-socket. Port: %s", "Any available port");
            }
            AddToSocketSelector(&m_tcpConnection);
            AddToSocketSelector(&m_udpConnection);
            StartListening();
            m_connState = ConnState::Connected;
            m_tryConnectDelay = sf::seconds(0.0f);
            m_connectMutex.unlock();
        }
        catch (const IException &e)
        {
            m_tcpConnection.GetSocket().disconnect();
            m_udpConnection.GetSocket().unbind();
            m_connectMutex.unlock();
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