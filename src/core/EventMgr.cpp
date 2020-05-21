#include "EventMgr.h"

std::vector<OnEventComponent *> EventMgr::m_callbacks;

void EventMgr::PollAll() noexcept
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        for (auto &callback : m_callbacks)
        {
            callback->OnEvent(event);
        }
    }
}

void EventMgr::AddOnEventFunction(OnEventComponent *callback) noexcept
{
    m_callbacks.push_back(callback);
}