#include "Keyboard.h"

std::unordered_map<sf::Keyboard::Key, bool> Keyboard::m_keymap;
std::unordered_map<sf::Keyboard::Key, bool> Keyboard::m_prevKeymap;
std::string Keyboard::m_textInputBuffer;

Keyboard::Keyboard()
{
    EventMgr::AddOnEventFunction(this);
}

Keyboard::~Keyboard()
{
}

void Keyboard::Update() noexcept
{
    for (auto &[key, state] : m_keymap)
        m_prevKeymap[key] = state;
    m_textInputBuffer.clear();
}

bool Keyboard::IsDown(const sf::Keyboard::Key &key) noexcept
{
    return m_keymap[key];
}

bool Keyboard::IsPressed(const sf::Keyboard::Key &key) noexcept
{
    return m_keymap[key] && !m_prevKeymap[key];
}

bool Keyboard::IsReleased(const sf::Keyboard::Key &key) noexcept
{
    return !m_keymap[key] && m_prevKeymap[key];
}

bool Keyboard::IsAnyDown() noexcept
{
    for (auto &[key, state] : m_keymap)
    {
        if (m_keymap[key])
            return true;
    }
    return false;
}

void Keyboard::OnEvent(const sf::Event &event) noexcept
{
    switch (event.type)
    {
    case sf::Event::EventType::KeyPressed:
    {
        OnPress(event.key.code);
        break;
    }
    case sf::Event::EventType::KeyReleased:
    {
        OnRelease(event.key.code);
        break;
    }
    case sf::Event::EventType::TextEntered:
    {
        OnTextInput(event.text.unicode);
        break;
    }
    }
}

void Keyboard::OnPress(const sf::Keyboard::Key &key) noexcept
{
    m_keymap[key] = true;
}

void Keyboard::OnRelease(const sf::Keyboard::Key &key) noexcept
{
    m_keymap[key] = false;
}

void Keyboard::OnTextInput(unsigned char character) noexcept
{
    m_textInputBuffer += character;
}