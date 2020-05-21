#pragma once

#include <unordered_map>

#include "EventMgr.h"

class Keyboard : public OnEventComponent
{
    enum class Button : unsigned char
    {
        Left,
        Middle,
        Right,
        X1,
        X2,
        Count,
    };

public:
    // Set up call-back function in eventMgr
    Keyboard();
    ~Keyboard();
    Keyboard(const Keyboard &) = delete;
    Keyboard &operator=(const Keyboard &) = delete;

    // Move keymap into prev-keymap
    static void UpdateKeyMaps() noexcept;

    static bool IsDown(const SDL_Keycode &key) noexcept;
    static bool IsPressed(const SDL_Keycode &key) noexcept;
    static bool IsReleased(const SDL_Keycode &key) noexcept;
    static bool IsAnyDown() noexcept;
    static std::string GetTextInput() noexcept { return m_textInputBuffer; }

private:
    virtual void OnEvent(const SDL_Event &event) noexcept override;

    static void OnPress(const SDL_Keycode &key) noexcept;
    static void OnRelease(const SDL_Keycode &key) noexcept;
    static void OnTextInput(const std::string &text) noexcept;

private:
    static std::unordered_map<SDL_Keycode, bool> m_keymap;
    static std::unordered_map<SDL_Keycode, bool> m_prevKeymap;
    static std::string m_textInputBuffer;
};