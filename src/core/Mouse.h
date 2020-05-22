#pragma once

#include <bitset>

#include <SFML/Window/Mouse.hpp>

#include "EventMgr.h"

class Mouse : public OnEventComponent
{

public:
    // Set up call-back function in eventMgr
    Mouse() noexcept;
    ~Mouse() noexcept;
    Mouse(const Mouse &) = delete;
    Mouse &operator=(const Mouse &) = delete;

    // Move buttonMap into prev-buttonMap
    static void Update() noexcept;

    static bool IsDown(const sf::Mouse::Button &button) noexcept;
    static bool IsPressed(const sf::Mouse::Button &button) noexcept;
    static bool IsReleased(const sf::Mouse::Button &button) noexcept;
    static bool AnyButtonDown() noexcept;

    static sf::Vector2i GetPos() noexcept { return m_mousePos; }
    static float GetVerticalScroll() noexcept { return m_verticalScrollBuffer; }
    static float GetHorizontalScroll() noexcept { return m_horizontalScrollBuffer; }

private:
    virtual void OnEvent(const sf::Event &event) noexcept override;

    static void OnPress(const sf::Mouse::Button &button) noexcept;
    static void OnRelease(const sf::Mouse::Button &button) noexcept;
    static void OnMove(int x, int y) noexcept;
    static void OnEnter() noexcept;
    static void OnLeave() noexcept;
    static void OnScroll(sf::Mouse::Wheel wheel, float delta) noexcept;

private:
    static std::unordered_map<sf::Mouse::Button, bool> m_buttonmap;
    static std::unordered_map<sf::Mouse::Button, bool> m_prevButtonmap;

    static sf::Vector2i m_mousePos;
    static bool m_inWindow;

    static float m_verticalScrollBuffer;
    static float m_horizontalScrollBuffer;
};