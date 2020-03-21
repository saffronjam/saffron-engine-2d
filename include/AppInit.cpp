#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "AppIncludes.hpp"

AppResources::AppResources(Application &app)
    : app(app),
      port(Random::Number(4000, 5000)),
      m_server(port),
      m_client("localhost", port)
{
    app.netMgr.SetClient(&m_client);
    app.netMgr.SetServer(&m_server);

    m_server.AcceptConnections();
    Time::sleep(0.5f);
    m_client.Connect();
    Time::sleep(0.5f);
    std::string send = "THIS IS A TEST";
    m_server.Broadcast<const char>(Query::Test, send.c_str(), send.size() + 1);
    m_client.Send<const char>(Query::Test, send.c_str(), send.size() + 1);
    Time::sleep(0.5f);
}