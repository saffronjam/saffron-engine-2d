#pragma once

#include <SFML/Graphics/Drawable.hpp>

#include "Window.h"

class Draw
{
public:
    static void Static(const sf::Drawable &drawable);
    static void Transformed(const sf::Drawable &drawable, const sf::Transform &transform);
    static void View(const sf::Drawable &drawable, const sf::View &view);
};