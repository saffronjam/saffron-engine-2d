#pragma once

#include <array>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Graphics.hpp"

class InputUtility
{
public:
    InputUtility(Graphics &gfx);

    void Update();

    bool IsToggled(const sf::Keyboard::Key key);
    bool IsToggled(const sf::Mouse::Button button);
    sf::Vector2i GetLastMouseSweep();

private:
    Graphics &m_gfx;

    std::array<bool, sf::Keyboard::Key::KeyCount> m_keys_lastState;
    std::array<bool, sf::Keyboard::Key::KeyCount> m_keys_wasReleased;
    std::array<bool, sf::Mouse::Button::ButtonCount> m_buttons_lastState;
    std::array<bool, sf::Mouse::Button::ButtonCount> m_buttons_wasReleased;

    sf::Vector2i m_last_pos;
    sf::Vector2i m_last_mouse_sweep;
};