#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "Core/AppIncludes.hpp"

AppResources::AppResources(Application &app)
    : app(app),
    port(Random::Generate(4000,5000)),
    m_server(port),
    m_client("localhost", port)
{
    m_server.AcceptConnections();
    Time::sleep(0.5f);
    m_client.Connect();
    Time::sleep(0.5f);
    std::string send = "THIS IS A TEST";
    m_server.Broadcast<std::string>("TEST", &send, sizeof(char) * 15);
    m_client.Send<std::string>("TEST", &send, sizeof(char) * 15);
    Time::sleep(0.5f);
    PacketT testClient = m_client.PopFront();
    PacketT testServer = m_server.PopFront();
    std::cout << "Query: " << testClient.query << " Data: " << *(std::string*)testClient.data << std::endl;
    std::cout << "Query: " << testServer.query << " Data: " <<  *(std::string*)testServer.data << std::endl;
}