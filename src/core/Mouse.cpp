#include "Mouse.h"

std::unordered_map<Mouse::Button, bool> Mouse::m_buttonmap;
std::unordered_map<Mouse::Button, bool> Mouse::m_prevButtonmap;
Vec2i Mouse::m_mousePos = Vec2i(0, 0);

Mouse::Mouse() noexcept
{
    EventMgr::AddOnEventFunction(this);
}

Mouse::~Mouse() noexcept
{
}

void Mouse::UpdateButtonMaps() noexcept
{
    for (auto &[key, state] : m_buttonmap)
        m_prevButtonmap[key] = state;
}

bool Mouse::IsDown(const Button &button) noexcept
{
    return m_buttonmap[button];
}

bool Mouse::IsPressed(const Button &button) noexcept
{
    return m_buttonmap[button] && !m_prevButtonmap[button];
}

bool Mouse::IsReleased(const Button &button) noexcept
{
    return !m_buttonmap[button] && m_prevButtonmap[button];
}

void Mouse::OnEvent(const SDL_Event &event) noexcept
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        Button button = static_cast<Button>(event.button.button);
        OnPress(button);
        break;
    }
    case SDL_MOUSEBUTTONUP:
    {
        Button button = static_cast<Button>(event.button.button);
        OnRelease(button);
        break;
    }
    case SDL_MOUSEMOTION:
    {
        m_mousePos.x = event.motion.x;
        m_mousePos.y = event.motion.y;
        break;
    }
    }
}

void Mouse::OnPress(const Button &button) noexcept
{
    m_buttonmap[button] = true;
}

void Mouse::OnRelease(const Button &button) noexcept
{
    m_buttonmap[button] = false;
}