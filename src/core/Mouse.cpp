#include "Mouse.h"

std::unordered_map<sf::Mouse::Button, bool> Mouse::m_buttonmap;
std::unordered_map<sf::Mouse::Button, bool> Mouse::m_prevButtonmap;
sf::Vector2i Mouse::m_mousePos = sf::Vector2i(0, 0);
bool Mouse::m_inWindow = true;
float Mouse::m_verticalScrollBuffer = 0.0f;
float Mouse::m_horizontalScrollBuffer = 0.0f;

Mouse::Mouse() noexcept
{
    EventMgr::AddOnEventFunction(this);
}

Mouse::~Mouse() noexcept
{
}

void Mouse::Update() noexcept
{
    for (auto &[key, state] : m_buttonmap)
        m_prevButtonmap[key] = state;

    m_horizontalScrollBuffer = 0.0f;
    m_verticalScrollBuffer = 0.0f;
}

bool Mouse::IsDown(const sf::Mouse::Button &button) noexcept
{
    return m_buttonmap[button];
}

bool Mouse::IsPressed(const sf::Mouse::Button &button) noexcept
{
    return m_buttonmap[button] && !m_prevButtonmap[button];
}

bool Mouse::IsReleased(const sf::Mouse::Button &button) noexcept
{
    return !m_buttonmap[button] && m_prevButtonmap[button];
}

bool Mouse::AnyButtonDown() noexcept
{
    for (auto &[button, state] : m_buttonmap)
        if (state)
            return true;
    return false;
}

void Mouse::OnEvent(const sf::Event &event) noexcept
{
    switch (event.type)
    {
    case sf::Event::EventType::MouseButtonPressed:
        OnPress(event.mouseButton.button);
        break;

    case sf::Event::EventType::MouseButtonReleased:
        OnRelease(event.mouseButton.button);
        break;

    case sf::Event::EventType::MouseMoved:
        OnMove(event.mouseMove.x, event.mouseMove.y);
        break;

    case sf::Event::EventType::MouseEntered:
        OnEnter();
        break;

    case sf::Event::EventType::MouseLeft:
        OnLeave();
        break;

    case sf::Event::EventType::MouseWheelScrolled:
        OnScroll(event.mouseWheelScroll.wheel, event.mouseWheelScroll.delta);
        break;
    }
}

void Mouse::OnPress(const sf::Mouse::Button &button) noexcept
{
    m_buttonmap[button] = true;
}

void Mouse::OnRelease(const sf::Mouse::Button &button) noexcept
{
    m_buttonmap[button] = false;
}

void Mouse::OnMove(int x, int y) noexcept
{
    if (!m_inWindow && AnyButtonDown() || m_inWindow)
    {
        m_mousePos.x = x;
        m_mousePos.y = y;
    }
}

void Mouse::OnEnter() noexcept
{
    m_inWindow = true;
}

void Mouse::OnLeave() noexcept
{
    m_inWindow = false;
}

void Mouse::OnScroll(sf::Mouse::Wheel wheel, float delta) noexcept
{
    if (wheel == sf::Mouse::Wheel::HorizontalWheel)
    {
        m_horizontalScrollBuffer += delta;
    }
    else if (wheel == sf::Mouse::Wheel::VerticalWheel)
    {
        m_verticalScrollBuffer += delta;
    }
}
