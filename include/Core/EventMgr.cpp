#include "EventMgr.hpp"

EventMgr::EventMgr(sf::RenderWindow &m_renderWindow, Input &input)
    : m_renderWindow(m_renderWindow),
      m_input(input)
{
    m_input.Reset();
}

void EventMgr::PollAll()
{
    while (m_renderWindow.pollEvent(m_event))
    {
        switch (m_event.type)
        {
        case sf::Event::Closed:
            m_input.Set(EVENT_QUIT, true);
            break;
        case sf::Event::KeyPressed:
        {
            switch (m_event.key.code)
            {
            case sf::Keyboard::Key::A:
                m_input.Set(KEY_A, true);
                break;
            case sf::Keyboard::Key::B:
                m_input.Set(KEY_B, true);
                break;
            case sf::Keyboard::Key::C:
                m_input.Set(KEY_C, true);
                break;
            case sf::Keyboard::Key::D:
                m_input.Set(KEY_D, true);
                break;
            case sf::Keyboard::Key::E:
                m_input.Set(KEY_E, true);
                break;
            case sf::Keyboard::Key::F:
                m_input.Set(KEY_F, true);
                break;
            case sf::Keyboard::Key::G:
                m_input.Set(KEY_G, true);
                break;
            case sf::Keyboard::Key::H:
                m_input.Set(KEY_H, true);
                break;
            case sf::Keyboard::Key::I:
                m_input.Set(KEY_I, true);
                break;
            case sf::Keyboard::Key::J:
                m_input.Set(KEY_J, true);
                break;
            case sf::Keyboard::Key::K:
                m_input.Set(KEY_K, true);
                break;
            case sf::Keyboard::Key::L:
                m_input.Set(KEY_L, true);
                break;
            case sf::Keyboard::Key::M:
                m_input.Set(KEY_M, true);
                break;
            case sf::Keyboard::Key::N:
                m_input.Set(KEY_N, true);
                break;
            case sf::Keyboard::Key::O:
                m_input.Set(KEY_O, true);
                break;
            case sf::Keyboard::Key::P:
                m_input.Set(KEY_P, true);
                break;
            case sf::Keyboard::Key::Q:
                m_input.Set(KEY_Q, true);
                break;
            case sf::Keyboard::Key::R:
                m_input.Set(KEY_R, true);
                break;
            case sf::Keyboard::Key::S:
                m_input.Set(KEY_S, true);
                break;
            case sf::Keyboard::Key::T:
                m_input.Set(KEY_T, true);
                break;
            case sf::Keyboard::Key::U:
                m_input.Set(KEY_U, true);
                break;
            case sf::Keyboard::Key::V:
                m_input.Set(KEY_V, true);
                break;
            case sf::Keyboard::Key::W:
                m_input.Set(KEY_W, true);
                break;
            case sf::Keyboard::Key::X:
                m_input.Set(KEY_X, true);
                break;
            case sf::Keyboard::Key::Y:
                m_input.Set(KEY_Y, true);
                break;
            case sf::Keyboard::Key::Z:
                m_input.Set(KEY_Z, true);
                break;
            case sf::Keyboard::Key::Escape:
                m_input.Set(KEY_ESC, true);
                break;
            default:
                break;
            }
            break;
        }
        case sf::Event::KeyReleased:
        {
            switch (m_event.key.code)
            {
            case sf::Keyboard::Key::A:
                m_input.Set(KEY_A, true);
                break;
            case sf::Keyboard::Key::B:
                m_input.Set(KEY_B, true);
                break;
            case sf::Keyboard::Key::C:
                m_input.Set(KEY_C, true);
                break;
            case sf::Keyboard::Key::D:
                m_input.Set(KEY_D, true);
                break;
            case sf::Keyboard::Key::E:
                m_input.Set(KEY_E, true);
                break;
            case sf::Keyboard::Key::F:
                m_input.Set(KEY_F, true);
                break;
            case sf::Keyboard::Key::G:
                m_input.Set(KEY_G, true);
                break;
            case sf::Keyboard::Key::H:
                m_input.Set(KEY_H, true);
                break;
            case sf::Keyboard::Key::I:
                m_input.Set(KEY_I, true);
                break;
            case sf::Keyboard::Key::J:
                m_input.Set(KEY_J, true);
                break;
            case sf::Keyboard::Key::K:
                m_input.Set(KEY_K, true);
                break;
            case sf::Keyboard::Key::L:
                m_input.Set(KEY_L, true);
                break;
            case sf::Keyboard::Key::M:
                m_input.Set(KEY_M, true);
                break;
            case sf::Keyboard::Key::N:
                m_input.Set(KEY_N, true);
                break;
            case sf::Keyboard::Key::O:
                m_input.Set(KEY_O, true);
                break;
            case sf::Keyboard::Key::P:
                m_input.Set(KEY_P, true);
                break;
            case sf::Keyboard::Key::Q:
                m_input.Set(KEY_Q, true);
                break;
            case sf::Keyboard::Key::R:
                m_input.Set(KEY_R, true);
                break;
            case sf::Keyboard::Key::S:
                m_input.Set(KEY_S, true);
                break;
            case sf::Keyboard::Key::T:
                m_input.Set(KEY_T, true);
                break;
            case sf::Keyboard::Key::U:
                m_input.Set(KEY_U, true);
                break;
            case sf::Keyboard::Key::V:
                m_input.Set(KEY_V, true);
                break;
            case sf::Keyboard::Key::W:
                m_input.Set(KEY_W, true);
                break;
            case sf::Keyboard::Key::X:
                m_input.Set(KEY_X, true);
                break;
            case sf::Keyboard::Key::Y:
                m_input.Set(KEY_Y, true);
                break;
            case sf::Keyboard::Key::Z:
                m_input.Set(KEY_Z, true);
                break;
            case sf::Keyboard::Key::Escape:
                m_input.Set(KEY_ESC, true);
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }
}