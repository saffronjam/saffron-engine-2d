#pragma once

#include <bitset>

#include "EventMgr.h"
#include "Vec2.h"

class Mouse : public OnEventComponent
{
public:
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
    Mouse() noexcept;
    ~Mouse() noexcept;
    Mouse(const Mouse &) = delete;
    Mouse &operator=(const Mouse &) = delete;

    // Move buttonMap into prev-buttonMap
    static void UpdateButtonMaps() noexcept;

    static bool IsDown(const Button &button) noexcept;
    static bool IsPressed(const Button &button) noexcept;
    static bool IsReleased(const Button &button) noexcept;
    static Vec2i GetPos() noexcept { return m_mousePos; }

private:
    virtual void OnEvent(const SDL_Event &event) noexcept override;

    static void OnPress(const Button &button) noexcept;
    static void OnRelease(const Button &button) noexcept;

private:
    static std::unordered_map<Button, bool> m_buttonmap;
    static std::unordered_map<Button, bool> m_prevButtonmap;

    static Vec2i m_mousePos;
};