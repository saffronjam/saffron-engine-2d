#pragma once

#define DEBUG

#include <SFML/Network.hpp>
#include <deque>
#include <vector>
#include <map>
#include <thread>
#include <iomanip>
#include <iostream>

#include "PacketT.hpp"

#ifdef SFML_SYSTEM_WINDOWS
        #include <winsock2.h>
#else
        #include <sys/socket.h>
#endif


class ClientT
{
public:
    ClientT(sf::TcpSocket *socket, std::string token)
        : socket(socket),
        token(token)
    {

    }
    sf::TcpSocket *socket;
    std::string token;
};

class ReuseableListener : public sf::TcpListener {
public:
    void reuse() {
            char reuse = 1;
            setsockopt( getHandle(), SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    }
};

class Server
{
public:
    Server();
    Server(int const &port);
    ~Server();

    int AcceptConnections();
    void RejectConnections();

    template <typename T>
    void Send(std::string query, T* data, size_t size, ClientT client)
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
        std::cout << "Info: Sending packet to " << m_clients[0].socket->getRemoteAddress() << std::endl;
#endif

        client.socket->send(packet);
    }

    template <typename T>
    void Broadcast(std::string query, T* data, size_t size)
    {
#ifdef DEBUG
        std::cout << "Info: Broadcasting packet..."<< std::endl;
#endif
        for(auto& client : m_clients)
        {
            Send<T>(query, data, size, client);
        }
#ifdef DEBUG
        std::cout << "Info: Broadcast finished..."<< std::endl;
#endif
    }

    PacketT PopFront();
    PacketT PopBack();

private:
    void Mgr();

private:
    int m_port;

    sf::SocketSelector m_socketSelector;
    ReuseableListener m_listener;
    std::vector<ClientT> m_clients;
    std::deque<std::pair<ClientT, PacketT>> m_pendingReceiving;
    std::deque<std::pair<ClientT, sf::Packet>> m_pendingSending;

    bool m_active;
    bool m_accepting;
    bool m_receiving;
    bool m_sending;

    std::thread m_worker;
};