#pragma once

#include <functional>
#include <vector>

#include <SFML/Window/Event.hpp>

#include "Window.h"

struct OnEventComponent
{
    virtual void OnEvent(const sf::Event &) = 0;
};

class EventMgr
{
public:
    static void PollAll() noexcept;
    static void AddOnEventFunction(OnEventComponent *callback) noexcept;

private:
    static std::vector<OnEventComponent *> m_callbacks;
};