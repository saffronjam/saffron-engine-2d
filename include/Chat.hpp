#pragma once

#include "Core/GUI.hpp"

class Chat
{
public:
    enum State
    {
        Login,
        StatesCount
    };

public:
    Chat(GUI &gui);

    void Update(sf::Time dt);

    void ChangeState(Chat::State newState);

private:
    void EnterState();
    void ExitState();

private:
    GUI &m_gui;

    State m_state;

    sfg::Window::Ptr m_login;
};