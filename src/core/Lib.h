#pragma once

#include <SFML/System/Vector2.hpp>

class Lib
{
public:
    template <typename T, typename U>
    static sf::Vector2<T> Vec2Convert(const sf::Vector2<U> &vec)
    {
        return sf::Vector2<T>((T)vec.x, (T)vec.y);
    }
};