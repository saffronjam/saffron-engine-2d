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
    m_parent.GetClient().SetNet("localhost", 1337);
    m_parent.GetClient().Connect();
}

void ClientMainScreen::OnExit()
{
}

void ClientMainScreen::Update()
{
    if (Keyboard::IsPressed(sf::Keyboard::P))
        m_parent.GetClient().SendArray<Protocol::TCP>(Text, "Hello, world!", 14);
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