#include "Chat.hpp"

Chat::Chat(GUI &gui)
    : m_gui(gui),
      m_login(sfg::Window::Create())
{
    m_login->SetTitle("Login");
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 0.5f);
    auto button = sfg::Button::Create("Login");
    box->Pack(button);

    button->GetSignal(sfg::Widget::OnLeftClick).Connect([&button] {
        button->SetLabel("Logged in");
    });

    m_login->Add(box);

    m_gui.AddWindow(m_login);

    ChangeState(Login);
}

void Chat::Update(sf::Time dt)
{
}

void Chat::ChangeState(Chat::State newState)
{
    ExitState();
    m_state = newState;
    EnterState();
}

void Chat::EnterState()
{
    switch (m_state)
    {
    case Login:
        m_gui.AddWindow(m_login);
        break;
    default:
        break;
    }
}
void Chat::ExitState()
{
    switch (m_state)
    {
    case Login:
        m_gui.RemoveWindow(m_login);
        break;
    default:
        break;
    }
}