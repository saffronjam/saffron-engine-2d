#include "INetMgr.h"

INetMgr::INetMgr()
    : m_uid(0),
      m_listening(false)
{
    PacketMgr::AddPacketBuffer(&m_inBuffer, &m_inBufferLock);
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

void INetMgr::AddToSocketSelector(sf::TcpListener *listener)
{
    log_info("Add listener to socket selector. Port: %u", listener->getLocalPort());
    m_tcpListenerRefs.emplace(listener);
    m_socketSelector.add(*listener);
}

void INetMgr::RemoveFromSocketSelector(const Connection *conn)
{
    log_info("Remove socket from socket selector");
    m_connectionRefs.erase(conn);
    m_socketSelector.remove(conn->GetTcpSocket());
    if (!conn->IsUdpParent())
        m_socketSelector.remove(conn->GetUdpSocket());
}

void INetMgr::RemoveFromSocketSelector(sf::TcpListener *listener)
{
    m_tcpListenerRefs.erase(listener);
    m_socketSelector.remove(*listener);
}

void INetMgr::AddNetModule(std::unique_ptr<INetModule> netModule)
{
    auto res = m_netModules.emplace(std::move(netModule));
    PacketMgr::AddHandler(res.first->get());
}

void INetMgr::ListenerThreadFn()
{
    while (m_listening)
    {
        bool anySocketReady = m_socketSelector.wait(sf::milliseconds(100));
        if (anySocketReady)
        {
            log_info("I think I received something...");
            for (auto &listener : m_tcpListenerRefs)
            {
                if (m_socketSelector.isReady(*listener))
                {
                    log_info("New tcp conn!");
                    NewTcpConnection(listener);
                    break;
                }
            }
            for (auto &conn : m_connectionRefs)
            {
                if (m_socketSelector.isReady(conn->GetTcpSocket()))
                {
                    log_info("New tcp pack!");
                    Receive<Protocol::TCP>(conn);
                    break;
                }
                else if (!conn->IsUdpParent() && m_socketSelector.isReady(conn->GetUdpSocket()))
                {
                    log_info("New udp pack!");
                    Receive<Protocol::UDP>(conn);
                    break;
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