#include "INetMgr.h"

INetMgr::INetMgr()
    : m_listening(false)
{
}

INetMgr::~INetMgr()
{
    StopListening();
    m_inBuffer.clear();
}

void INetMgr::StartListening()
{
    m_listenerThread = std::thread(&INetMgr::ListenerThreadFn, this);
    m_listening = true;
}

void INetMgr::StopListening()
{
    m_listening = false;
    if (m_listenerThread.joinable())
        m_listenerThread.join();
}

void INetMgr::AddToSocketSelector(const IConnection *iconn)
{
    m_connectionRefs.emplace(iconn);
    m_socketSelector.add(iconn->GetISocket());
}

void INetMgr::RemoveFromSocketSelector(const IConnection *iconn)
{
    m_connectionRefs.emplace(iconn);
    m_socketSelector.remove(iconn->GetISocket());
}

void INetMgr::AddNetModule(std::unique_ptr<INetModule> netModule)
{
    auto res = m_netModules.emplace(std::move(netModule));
    PacketMgr::AddHandler(res.first->get());
}

void INetMgr::Receive(const IConnection *iconn)
{
    sf::Packet incoming;
    const Connection<sf::TcpSocket> *tcpConn;
    const Connection<sf::UdpSocket> *udpConn;
    if ((tcpConn = dynamic_cast<const Connection<sf::TcpSocket> *>(iconn)))
    {
        sf::Socket::Status status;
        while ((status = tcpConn->GetSocket().receive(incoming)) != sf::Socket::Done)
        {
            if (status != sf::Socket::Partial)
            {
                return;
            }
        }

        if (incoming.getDataSize() < sizeof(PacketType))
        {
            return;
        }
    }
    else if ((udpConn = dynamic_cast<const Connection<sf::UdpSocket> *>(iconn)))
    {
        sf::IpAddress ip = udpConn->GetRemoteAddress();
        sf::Uint16 port = udpConn->GetRemotePort();
        sf::Socket::Status status;
        while ((status = udpConn->GetSocket().receive(incoming, ip, port)) != sf::Socket::Done)
        {
            if (status != sf::Socket::Partial)
            {
                return;
            }
        }
        if (incoming.getDataSize() < sizeof(PacketType))
        {
            return;
        }
    }
    else
    {
        return;
    }

    std::optional<ParsedPacket> parseAttempt;
    if (tcpConn)
        parseAttempt = Packager::Parse(incoming, tcpConn);
    else
        parseAttempt = Packager::Parse(incoming, udpConn);
    if (parseAttempt.has_value())
    {
        m_inBufferLock.lock();
        m_inBuffer.push_back(parseAttempt.value());
        m_inBufferLock.unlock();
    }
    return;
}

void INetMgr::ListenerThreadFn()
{
    while (m_listening)
    {
        bool anySocketReady = m_socketSelector.wait(sf::milliseconds(100));
        if (anySocketReady)
        {
            for (auto &iconn : m_connectionRefs)
            {
                if (m_socketSelector.isReady(iconn->GetISocket()))
                {
                    if (dynamic_cast<const Connection<sf::TcpListener> *>(iconn))
                        NewConnection(dynamic_cast<const Connection<sf::TcpListener> *>(iconn));
                    else
                        Receive(iconn);
                }
            }
        }
    }
}

INetMgr::Exception::Exception(int line, const char *file, const char *errorString) noexcept
    : IException(line, file),
      errorString(errorString)
{
}

const char *INetMgr::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << "[Type] " << GetType() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
const char *INetMgr::Exception::GetType() const noexcept
{
    return "V-2DEngine NetMgr Exception";
}
const char *INetMgr::Exception::GetErrorString() const noexcept
{
    return errorString.c_str();
}