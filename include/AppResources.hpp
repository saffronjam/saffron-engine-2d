#pragma once

#include "Core/AppIncludes.hpp"

class AppResources
{
public:
    AppResources(class Application &app);
    ~AppResources();
    void Draw();
    void Update();

private:
    class Application &app;

private:
    //Write here
    Server m_server;
    std::array<Client, 5> m_clients;
};