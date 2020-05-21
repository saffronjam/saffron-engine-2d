#include "Keyboard.h"

std::unordered_map<SDL_Keycode, bool> Keyboard::m_keymap;
std::unordered_map<SDL_Keycode, bool> Keyboard::m_prevKeymap;
std::string Keyboard::m_textInputBuffer;

Keyboard::Keyboard()
{
    EventMgr::AddOnEventFunction(this);
}

Keyboard::~Keyboard()
{
}

void Keyboard::UpdateKeyMaps() noexcept
{
    for (auto &[key, state] : m_keymap)
        m_prevKeymap[key] = state;
}

bool Keyboard::IsDown(const SDL_Keycode &key) noexcept
{
    return m_keymap[key];
}

bool Keyboard::IsPressed(const SDL_Keycode &key) noexcept
{
    return m_keymap[key] && !m_prevKeymap[key];
}

bool Keyboard::IsReleased(const SDL_Keycode &key) noexcept
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

void Keyboard::OnEvent(const SDL_Event &event) noexcept
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
    {
        OnPress(event.key.keysym.sym);
        break;
    }
    case SDL_KEYUP:
    {
        OnRelease(event.key.keysym.sym);
        break;
    }
    case SDL_TEXTINPUT:
    {
        OnTextInput(event.text.text);
        break;
    }
    }
}

void Keyboard::OnPress(const SDL_Keycode &key) noexcept
{
    m_keymap[key] = true;
}

void Keyboard::OnRelease(const SDL_Keycode &key) noexcept
{
    m_keymap[key] = false;
}

void Keyboard::OnTextInput(const std::string &text) noexcept
{
    m_textInputBuffer.clear();
    m_textInputBuffer = text;
}