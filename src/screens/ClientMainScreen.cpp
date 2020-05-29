#include "ClientMainScreen.h"
#include "AppClient.h"

ClientMainScreen::ClientMainScreen(AppClient &parent)
    : m_parent(parent)
{
}

ClientMainScreen::~ClientMainScreen()
{
}

void ClientMainScreen::Build()
{
}

void ClientMainScreen::Destroy()
{
}

void ClientMainScreen::OnEntry()
{
    FPSLimiter::SetDesiredFPS(60.0f);
    m_parent.GetClient().SetNet("localhost", 1332);
    m_parent.GetClient().Connect();
    m_parent.GetClient().EnableAutoReconnect();
}

void ClientMainScreen::OnExit()
{
}

void ClientMainScreen::Update()
{
    if (Keyboard::IsPressed(sf::Keyboard::T))
    {
        log_info("Asking server if he is alive... with TCP!");
        m_parent.GetClient().SendEmpty<TCP>(AreYouAlive);
    }
    if (Keyboard::IsPressed(sf::Keyboard::U))
    {
        log_info("Asking server if he is alive... with UDP!");
        m_parent.GetClient().SendEmpty<UDP>(AreYouAlive);
    }
}

void ClientMainScreen::Draw()
{
}

int ClientMainScreen::GetNextScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}

int ClientMainScreen::GetPreviousScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}