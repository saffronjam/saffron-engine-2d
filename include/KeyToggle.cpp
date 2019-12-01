#include "KeyToggle.hpp"

using namespace sf;

KeyToggle::KeyToggle()
{
    for (int i = 0; i < sf::Keyboard::Key::KeyCount; i++)
    {
        m_keys_lastState[i] = false;
        m_keys_wasReleased[i] = true;
    }
}

void KeyToggle::Update()
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
}

bool KeyToggle::IsToggled(sf::Keyboard::Key key)
{
    return m_keys_lastState[key];
}