#pragma once

#include <array>

enum Code
{
    EVENT_QUIT,
    MOUSE_L,
    MOUSE_R,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_ESC,
    CODE_nCodes
};

class Input
{
public:
    Input();

    void Reset();
    bool Get(const Code key);
    void Set(const Code key, bool newState);

private:
    std::array<bool, CODE_nCodes> m_events;
};