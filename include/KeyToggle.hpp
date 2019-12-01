#pragma once

#include <array>
#include <SFML/Window/Keyboard.hpp>

class KeyToggle
{
public:
    KeyToggle();

    void Update();

    bool IsToggled(sf::Keyboard::Key key);

private:
    std::array<bool, sf::Keyboard::Key::KeyCount> m_keys_lastState;
    std::array<bool, sf::Keyboard::Key::KeyCount> m_keys_wasReleased;
};