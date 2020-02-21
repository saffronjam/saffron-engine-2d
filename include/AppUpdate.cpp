#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "Core/AppIncludes.hpp"

void AppResources::Update()
{

    if (app.input.Get(KeyA))
        std::cout << (char *)m_server.PopFront().getData() << std::endl;
}