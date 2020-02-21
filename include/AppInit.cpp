#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "Core/AppIncludes.hpp"

AppResources::AppResources(Application &app)
    : app(app)
{
    m_server.AcceptConnections();
    for (auto &client : m_clients)
    {
        client.Connect();
        client.Send("This is a test");
    }
}