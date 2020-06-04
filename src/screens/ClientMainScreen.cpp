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
}

void ClientMainScreen::Draw()
{
    sf::RectangleShape rect;
    rect.setPosition(Mouse::GetPos());
    rect.setSize(sf::Vector2f(50.0f, 50.0f));
    rect.setFillColor(sf::Color::Green);
    Window::Draw(rect);
}

int ClientMainScreen::GetNextScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}

int ClientMainScreen::GetPreviousScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}