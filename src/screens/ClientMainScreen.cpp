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
    m_parent.GetClient().SetNet("localhost", 1330);
    m_parent.GetClient().Connect();
}

void ClientMainScreen::OnExit()
{
}

void ClientMainScreen::Update()
{
    if (Keyboard::IsPressed(sf::Keyboard::P))
    {
        log_info("Asking server if he is alive...");
        m_parent.GetClient().SendEmpty<TCP>(AreYouAlive);
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