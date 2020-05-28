#include "Connection.h"

Connection::Connection(const sf::IpAddress &udpRemoteAddress, const sf::Uint16 &udpRemotePort)
    : m_tcpSocket(new sf::TcpSocket()),
      m_udpSocket(new sf::UdpSocket()),
      m_udpSocketChild(std::nullopt),
      m_udpRemoteAddress(udpRemoteAddress),
      m_udpRemotePort(udpRemotePort)
{
}

Connection::Connection(sf::UdpSocket *udpSocket, const sf::IpAddress &udpRemoteAddress, const sf::Uint16 &udpRemotePort)
    : m_tcpSocket(new sf::TcpSocket()),
      m_udpSocketChild(udpSocket),
      m_udpRemoteAddress(udpRemoteAddress),
      m_udpRemotePort(udpRemotePort)
{
}

Connection::~Connection()
{
    m_tcpSocket = nullptr;
    m_udpSocket = std::nullopt;
    m_udpSocketChild = std::nullopt;
}

bool Connection::operator<(const Connection &rhs) const
{
    return m_tcpSocket < rhs.m_tcpSocket;
}