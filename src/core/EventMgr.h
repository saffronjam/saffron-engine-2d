#pragma once

#include <functional>
#include <vector>

#include <SDL2/SDL_events.h>

struct OnEventComponent
{
    virtual void OnEvent(const SDL_Event &) = 0;
};

class EventMgr
{
public:
    static void PollAll() noexcept;
    static void AddOnEventFunction(OnEventComponent *callback) noexcept;

private:
    static std::vector<OnEventComponent *> m_callbacks;
};