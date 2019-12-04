#include "InputUtility.hpp"

using namespace sf;

InputUtility::InputUtility(Graphics &gfx)
    : m_gfx(gfx),
      m_last_pos(m_gfx.GetMousePosition()),
      m_last_mouse_sweep(0, 0)
{
    for (int i = 0; i < sf::Keyboard::Key::KeyCount; i++)
    {
        m_keys_lastState[i] = false;
        m_keys_wasReleased[i] = true;
    }
}

void InputUtility::Update()
{
    for (int i = 0; i < sf::Keyboard::Key::KeyCount; i++)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i)) && m_keys_wasReleased[i])
        {
            m_keys_wasReleased[i] = false;
            m_keys_lastState[i] ? m_keys_lastState[i] = false : m_keys_lastState[i] = true;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i)))
        {
            m_keys_wasReleased[i] = true;
        }
    }

    for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button(i)) && m_buttons_wasReleased[i])
        {
            m_buttons_wasReleased[i] = false;
            m_buttons_lastState[i] ? m_buttons_lastState[i] = false : m_buttons_lastState[i] = true;
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button(i)))
        {
            m_buttons_wasReleased[i] = true;
        }
    }

    const sf::Vector2i cur_pos = m_gfx.GetMousePosition();
    const sf::Vector2i delta = cur_pos - m_last_pos;
    if (vf::LengthSq((sf::Vector2f)delta) > 0.0f)
    {
        m_last_pos = cur_pos;
        m_last_mouse_sweep = delta;
    }
}

bool InputUtility::IsToggled(const sf::Keyboard::Key key)
{
    return m_keys_lastState[key];
}

bool InputUtility::IsToggled(const sf::Mouse::Button button)
{
    return m_buttons_lastState[button];
}

sf::Vector2i InputUtility::GetLastMouseSweep()
{
    return m_last_mouse_sweep;
}