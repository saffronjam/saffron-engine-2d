#pragma once

#include <unordered_map>

#include "EventMgr.h"

class Keyboard : public EventHandler
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
    static void Update() noexcept;

    static bool IsDown(const sf::Keyboard::Key &key) noexcept;
    static bool IsPressed(const sf::Keyboard::Key &key) noexcept;
    static bool IsReleased(const sf::Keyboard::Key &key) noexcept;
    static bool IsAnyDown() noexcept;
    static std::string GetTextInput() noexcept { return m_textInputBuffer; }

private:
    virtual void OnEvent(const sf::Event &event) noexcept override;

    static void OnPress(const sf::Keyboard::Key &key) noexcept;
    static void OnRelease(const sf::Keyboard::Key &key) noexcept;
    static void OnTextInput(unsigned char character) noexcept;

private:
    static std::unordered_map<sf::Keyboard::Key, bool> m_keymap;
    static std::unordered_map<sf::Keyboard::Key, bool> m_prevKeymap;
    static std::string m_textInputBuffer;
};