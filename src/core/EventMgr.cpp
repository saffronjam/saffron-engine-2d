#include "EventMgr.h"

std::vector<EventHandler *> EventMgr::m_callbacks;

void EventMgr::PollAll()
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

void EventMgr::AddOnEventCallback(EventHandler *callback)
{
    m_callbacks.push_back(callback);
}