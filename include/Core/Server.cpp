#include "Server.hpp"

#include <iostream>
#include <cstring>



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
}

Server::~Server()
{
    m_active = false;
    m_accepting = false;
    m_receiving = false;
    m_sending = false;

    for (auto &client : m_clients)
        client.socket->disconnect();

    m_socketSelector.clear();

    if (m_worker.joinable())
        m_worker.join();

    for (auto &client : m_clients)
    {
        delete client.socket;
        client.socket = nullptr;
    }
}

int Server::AcceptConnections()
{
    m_listener.reuse();
    if (m_listener.listen(m_port) == sf::Socket::Done)
    {
#ifdef DEBUG
        std::cout << "Info: Successfully started listener on server! Port: " << m_port << std::endl;
#endif
        m_socketSelector.add(m_listener);
        m_accepting = true;
        return 1;
    }
    else
    {
#ifdef DEBUG
        std::cerr << "Error: Failed to start listener on server! Port: " << m_port << std::endl;
#endif
        return 0;
    }
}

void Server::RejectConnections()
{
    m_listener.close();
    m_accepting = false;
}

PacketT Server::PopFront()
{
    if (m_pendingReceiving.size() > 0)
    {
        PacketT ret(m_pendingReceiving.front().second);
        m_pendingReceiving.pop_front();
        return ret;
    }
    else
    {
        return PacketT("EMPTY", nullptr, 0);
    }
}

PacketT Server::PopBack()
{
    if (m_pendingReceiving.size() > 0)
    {
        PacketT ret(m_pendingReceiving.back().second);
        m_pendingReceiving.pop_back();
        return ret;
    }
    else
    {
        return PacketT("EMPTY", nullptr, 0);
    }
}

void Server::Mgr()
{
    while (m_active)
    {
        if (m_socketSelector.wait(sf::milliseconds(10)))
        {
            if (m_accepting && m_socketSelector.isReady(m_listener))
            {
                sf::TcpSocket *clientSocket = new sf::TcpSocket;
                if (m_listener.accept(*clientSocket) == sf::Socket::Done)
                {
#ifdef DEBUG
                    std::cout << "Info: Successfully accepted connection to client! IP: " << clientSocket->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                    m_clients.push_back(ClientT(clientSocket,""));
                    m_socketSelector.add(*clientSocket);
                }
                else
                {
#ifdef DEBUG
                    std::cerr << "Error: Failed to accept connection to client! IP: " << clientSocket->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                    delete clientSocket;
                }
            }
            else if (m_receiving)
            {
                for (auto &client : m_clients)
                {
                    if (m_socketSelector.isReady(*client.socket))
                    {
                        sf::Packet packet;
                        if (client.socket->receive(packet) == sf::Socket::Done)
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
                            std::cout << "Info: Successfully received packet from client! IP: " << client.socket->getRemoteAddress() << " Port: " << m_port  << " Query: " << query<< std::endl;
#endif
                            m_pendingReceiving.push_back(std::make_pair(client, PacketT(query, data, dataSize)));
                        }
                        else
                        {
#ifdef DEBUG
                            std::cerr << "Error: Failed to receive packet from client! Did the client disconnect? IP: " << client.socket->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                        }
                    }
                }
            }
        }
        if (m_sending)
            while (m_pendingSending.size() > 0)
            {
                ClientT client = m_pendingSending.front().first;
                sf::Packet *packet = &m_pendingSending.front().second;

                if (client.socket->send(*packet) == sf::Socket::Done)
                {
#ifdef DEBUG
                    std::cout << "Info: Successfully sent packet to server! IP: " << client.socket->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                    m_pendingSending.pop_front();
                }
                else
                {
#ifdef DEBUG
                    std::cerr << "Error: Failed to send packet to server! IP: " << client.socket->getRemoteAddress() << " Port: " << m_port << std::endl;
#endif
                }
            }
    }
}