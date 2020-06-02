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
}

void ClientMainScreen::OnExit()
{
}

void ClientMainScreen::Update()
{
    m_camera.Update();
}

void ClientMainScreen::Draw()
{
    sf::RectangleShape tmp;
    tmp.setPosition(-1.0f, -1.0f);
    tmp.setSize(sf::Vector2f(1.0f, 1.0f));
    tmp.setFillColor(sf::Color::Red);
    m_camera.Draw(tmp);
    Window::Draw(tmp);
}

int ClientMainScreen::GetNextScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}

int ClientMainScreen::GetPreviousScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}