#pragma once

#define DEBUG

#include <deque>
#include <thread>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <SFML/Network.hpp>

#include "PacketT.hpp"


class ServerT
{
public:
    ServerT(sf::TcpSocket* socket, std::string token)
        : socket(socket),
        token(token)
    {    
    }
    sf::TcpSocket* socket;
    std::string token;
};

class Client
{
public:
    Client();
    Client(sf::IpAddress const &hostIp, int const &port);
    ~Client();

    int Connect();
    void Disconnect();

    template <typename T>
    void Send(std::string query, T* data, size_t size)
    {
        sf::Packet packet;

        std::stringstream ss;
        ss << std::left << std::setw(HEADER_SIZE) << size;
        std::string header_formatted = ss.str();
        ss.str("");
        ss.clear();
        ss << std::left << std::setw(QUERY_SIZE) << query;
        std::string query_formatted = ss.str();

        packet.append((void*)header_formatted.c_str(), header_formatted.length() * sizeof(char));
        packet.append((void*)query_formatted.c_str(), query_formatted.length() * sizeof(char));
        packet.append((void*)data, size);

#ifdef DEBUG
        std::cout << "Info: Sending packet to " << m_server.socket->getRemoteAddress() << std::endl;
#endif

        m_server.socket->send(packet);
    }

    PacketT PopFront();
    PacketT PopBack();

private:
    void Mgr();

private:
    sf::IpAddress m_ip;
    int m_port;

    sf::SocketSelector m_socketSelector;
    ServerT m_server;

    std::deque<PacketT> m_pendingReceiving;
    std::deque<sf::Packet> m_pendingSending;

    bool m_active;
    bool m_receiving;
    bool m_sending;
    bool m_connected;

    std::thread m_worker;
};