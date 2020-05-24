#include "EventMgr.h"

std::vector<EventHandler *> EventMgr::m_callbacks;

void EventMgr::PollAll() noexcept
{
    sf::Event event;
    while (Window::GetSFWindow()->pollEvent(event))
    {
        for (auto &callback : m_callbacks)
        {
            callback->OnEvent(event);
        }
    }
}

void EventMgr::AddOnEventFunction(EventHandler *callback) noexcept
{
    m_callbacks.push_back(callback);
}