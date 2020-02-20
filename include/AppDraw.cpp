#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "Core/AllSFML.hpp"

void AppResources::Draw()
{
    app.gfx.DrawLine(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(200.0f, 300.0f), sf::Color::White);
}