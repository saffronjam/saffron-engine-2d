#include "SaffronPCH.h"

#include "Saffron/Core/Clock.h"

namespace Se
{
Clock::Clock() :
	_dt(sf::Time::Zero)
{
}

auto Clock::FrameTime() const -> sf::Time
{
	return _dt;
}

auto Clock::ElapsedTime() const -> sf::Time
{
	return _clock.getElapsedTime();
}

auto Clock::Restart() -> sf::Time
{
	_start += ElapsedTime();
	_dt = _clock.restart();
	return _dt;
}

auto Clock::SinceStart() const -> sf::Time
{
	return _start + ElapsedTime();
}
}
