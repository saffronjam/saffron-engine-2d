#include "GameplayScreen.h"

GameplayScreen::GameplayScreen()
{
}

GameplayScreen::~GameplayScreen()
{
}

void GameplayScreen::Build()
{
}

void GameplayScreen::Destroy()
{
}

void GameplayScreen::OnEntry()
{
}

void GameplayScreen::OnExit()
{
}

void GameplayScreen::Update()
{
}

void GameplayScreen::Draw()
{
    sf::RectangleShape rectangle = sf::RectangleShape();
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setSize(sf::Vector2f(20.0f, 20.0f));
    rectangle.setPosition(Lib::Vec2Convert<float, int>(Mouse::GetPos()));

    sf::Transform transform = sf::Transform::Identity;
    transform.rotate((scroll += Mouse::GetHorizontalScroll()));

    Draw::Transformed(rectangle, transform);

    if (Keyboard::IsPressed(sf::Keyboard::Space))
    {
        Window::SetFullscreen(true);
    }
    else if (Keyboard::IsReleased(sf::Keyboard::Space))
    {
        Window::SetFullscreen(false);
    }
}

int GameplayScreen::GetNextScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}

int GameplayScreen::GetPreviousScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}