#pragma once

#include <functional>
#include <vector>

#include <SFML/Window/Event.hpp>

#include "Window.h"

struct EventHandler
{
    virtual void OnEvent(const sf::Event &) = 0;
};

class EventMgr
{
public:
    static void PollAll() noexcept;
    static void AddOnEventFunction(EventHandler *callback) noexcept;

private:
    static std::vector<EventHandler *> m_callbacks;
};