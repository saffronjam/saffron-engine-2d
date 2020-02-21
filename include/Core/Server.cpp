#include "Server.hpp"

#include <iostream>

Server::Server()
    : Server(4000)
{
}

Server::Server(int const &port)
    : m_port(port),
      m_active(true),
      m_accepting(false),
      m_receiving(true),
      m_sending(true),
      m_worker(&Server::Mgr, this)
{
    if (m_listener.listen(m_port) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not bind listener to port" << std::endl;
    }
    m_socketSelector.add(m_listener);
}

Server::~Server()
{
    m_active = false;
    m_accepting = false;
    m_receiving = false;
    m_sending = false;

    m_socketSelector.clear();

    if (m_worker.joinable())
        m_worker.join();

    for (auto &client : m_clients)
    {
        delete client;
        client = nullptr;
    }
}

void Server::AcceptConnections()
{
    m_accepting = true;
}

void Server::RejectConnections()
{
    m_accepting = false;
}

void Server::Send(sf::Packet const &packet, sf::TcpSocket *client)
{
    m_pendingSending.push_back(std::make_pair(client, packet));
}

void Server::Send(void *data, size_t size, sf::TcpSocket *client)
{
    sf::Packet packet;
    packet.append(data, size);
    Send(packet, client);
}

void Server::Send(std::string const &string, sf::TcpSocket *client)
{
    sf::Packet packet;
    packet.append((void *)&string, string.length() + 1);
    Send(packet, client);
}

sf::Packet Server::PopFront()
{
    if (m_pendingReceiving.size() > 0)
    {
        sf::Packet ret = m_pendingReceiving.front().second;
        m_pendingReceiving.pop_front();
        return ret;
    }
    else
    {
        sf::Packet ret;
        ret.append("", 1);
        return ret;
    }
}

sf::Packet Server::PopBack()
{
    if (m_pendingReceiving.size() > 0)
    {
        sf::Packet ret = m_pendingReceiving.back().second;
        m_pendingReceiving.pop_back();
        return ret;
    }
    else
    {
        sf::Packet ret;
        ret.append("", 1);
        return ret;
    }
}

void Server::Mgr()
{
    while (m_active)
    {
        if (m_socketSelector.wait(sf::milliseconds(1000)))
        {
            if (m_accepting && m_socketSelector.isReady(m_listener))
            {
                sf::TcpSocket *client = new sf::TcpSocket;
                if (m_listener.accept(*client) == sf::Socket::Done)
                {
                    m_clients.push_back(client);
                    m_socketSelector.add(*client);
                }
                else
                    delete client;
            }
            else if (m_receiving)
            {
                for (auto &client : m_clients)
                {
                    if (m_socketSelector.isReady(*client))
                    {
                        sf::Packet packet;
                        if (client->receive(packet) == sf::Socket::Done)
                        {
                            m_pendingReceiving.push_back(std::make_pair(client, packet));
                        }
                    }
                }
            }
        }
        if (m_sending)
            while (m_pendingSending.size() > 0)
            {
                m_pendingSending.front().first->send(m_pendingSending.front().second);
                m_pendingSending.pop_front();
            }
    }
}