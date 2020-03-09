#include "Client.hpp"
#include <iostream>
#include <cstring>

using namespace std::chrono_literals;

Client::Client()
    : Client(sf::IpAddress("127.0.0.1"), 4000)
{
}

Client::Client(sf::IpAddress const &ip, int const &port)
    : m_ip(ip),
      m_port(port),
      m_server(ServerT(new sf::TcpSocket, "")),
      m_active(true),
      m_receiving(true),
      m_sending(true),
      m_connected(false),
      m_worker(&Client::Mgr, this)
{
}

Client::~Client()
{
    m_active = false;
    m_receiving = false;
    m_sending = false;

    Disconnect();

    m_socketSelector.clear();

    if (m_worker.joinable())
        m_worker.join();

    delete m_server.socket;
}

int Client::Connect()
{
    if (!m_connected)
    {
        if (m_server.socket->connect(m_ip, m_port) == sf::Socket::Status::Done)
        {
            m_socketSelector.add(*m_server.socket);
            m_connected = true;
            return 1;
        }
    }
    return 0;
}

void Client::Disconnect()
{
    if (m_connected)
    {
        m_server.socket->disconnect();
        m_connected = false;
    }
}

PacketT Client::PopFront()
{
    if (m_pendingReceiving.size() > 0)
    {
        PacketT ret(m_pendingReceiving.front());
        m_pendingReceiving.pop_front();
        return ret;
    }
    else
    {
        return PacketT("EMPTY", nullptr, 0);
    }
}

PacketT Client::PopBack()
{
    if (m_pendingReceiving.size() > 0)
    {
        PacketT ret(m_pendingReceiving.back());
        m_pendingReceiving.pop_back();
        return ret;
    }
    else
    {
        return PacketT("EMPTY", nullptr, 0);
    }
}

void Client::Mgr()
{
    while (m_active)
    {
        if (m_socketSelector.wait(sf::milliseconds(100)))
        {
            if (m_receiving && m_socketSelector.isReady(*m_server.socket))
            {
                sf::Packet packet;
                if (m_server.socket->receive(packet) == sf::Socket::Done)
                {
                    char* raw_msg = (char*)packet.getData();

                    char header_raw[HEADER_SIZE];
                    memcpy(header_raw, raw_msg, sizeof(char) * HEADER_SIZE);
                    header_raw[HEADER_SIZE-1] = '\0';
                    std::string header(header_raw);
                    header.erase(std::remove_if(header.begin(), header.end(), isspace), header.end());
                    int dataSize = std::stoi(header);

                    char query_raw[QUERY_SIZE];
                    memcpy(query_raw, raw_msg + HEADER_SIZE, sizeof(char) * QUERY_SIZE);
                    std::string query(query_raw);
                    query.erase(std::remove_if(query.begin(), query.end(), isspace), query.end());

                    void* data = malloc(dataSize);
                    memcpy(data, raw_msg + HEADER_SIZE + QUERY_SIZE, sizeof(char) * dataSize);
#ifdef DEBUG
                    std::cout << "Info: Successfully received packet from server! IP: " << m_ip << " Port: " << m_port << " Query: " << query << std::endl;
#endif
                    m_pendingReceiving.push_back(PacketT(query, data, dataSize));
                }
                else
                {
#ifdef DEBUG
                    std::cerr << "Error: Failed to receive packet from server! IP: " << m_ip << " Port: " << m_port << std::endl;
#endif
                }
            }
        }
        if (m_sending)
            while (m_pendingSending.size() > 0)
            {
                if (m_server.socket->send(m_pendingSending.front()) == sf::Socket::Done)
                {
#ifdef DEBUG
                    std::cout << "Info: Successfully sent packet to server! IP: " << m_ip << " Port: " << m_port << std::endl;
#endif
                    m_pendingSending.pop_front();
                }
                else
                {
#ifdef DEBUG
                    std::cerr << "Error: Failed to send packet to server! IP: " << m_ip << " Port: " << m_port << std::endl;
#endif
                }
            }
    }
}