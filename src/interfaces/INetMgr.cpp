#include "INetMgr.h"

INetMgr::INetMgr()
    : m_listening(false)
{
}

INetMgr::~INetMgr()
{
    StopListening();
    for (auto &parsedPacket : m_inBuffer)
    {
        parsedPacket.type = PacketType::Text;
        if (parsedPacket.data)
            delete parsedPacket.data;
        parsedPacket.dataSize = 0;
        parsedPacket.fullSize = 0;
    }
}

void INetMgr::StartListening()
{
    m_listenerThread = std::thread(&ListenerThreadFn, this);
    m_listening = true;
}

void INetMgr::StopListening()
{
    m_listening = false;
    if (m_listenerThread.joinable())
        m_listenerThread.join();
}

void INetMgr::ListenerThreadFn()
{
    while (m_listening)
    {
        bool anySocketReady = m_socketSelector.wait();
        if (anySocketReady)
        {
            for (auto &conn : m_allTcpListenerRefs)
            {
                if (m_socketSelector.isReady(conn->GetSocket()))
                    NewConnection(conn);
            }
            for (auto &conn : m_allTcpSocketRefs)
            {
                if (m_socketSelector.isReady(conn->GetSocket()))
                    Receive(conn);
            }
            for (auto &conn : m_allUdpSocketRefs)
            {
                if (m_socketSelector.isReady(conn->GetSocket()))
                    Receive(conn);
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