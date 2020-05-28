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