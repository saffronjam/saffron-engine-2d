#include "ServerMainScreen.h"
#include "AppServer.h"

ServerMainScreen::ServerMainScreen(AppServer &parent)
    : m_parent(parent)
{
}

ServerMainScreen::~ServerMainScreen()
{
}

void ServerMainScreen::Build()
{
}

void ServerMainScreen::Destroy()
{
}

void ServerMainScreen::OnEntry()
{
    m_parent.GetServer().SetPort(1330);
    m_parent.GetServer().Open();
}

void ServerMainScreen::OnExit()
{
}

void ServerMainScreen::Update()
{
    if (Keyboard::IsPressed(sf::Keyboard::P))
    {
        log_info("Asking client if he is alive...");
        m_parent.GetServer().BroadcastEmpty<TCP>(AreYouAlive);
    }
}

void ServerMainScreen::Draw()
{
}

int ServerMainScreen::GetNextScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}

int ServerMainScreen::GetPreviousScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}