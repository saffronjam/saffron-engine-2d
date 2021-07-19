#include "SaffronPCH.h"

#include "Saffron/Core/Global.h"

namespace Se
{
Global::Clock::LocalClock Global::Clock::_localClock;

auto Global::Clock::FrameTime() -> sf::Time
{
	return _localClock.FrameTime();
}

auto Global::Clock::ElapsedTime() -> sf::Time
{
	return _localClock.ElapsedTime();
}

auto Global::Clock::Restart() -> sf::Time
{
	return _localClock.Restart();
}

auto Global::Clock::SinceStart() -> sf::Time
{
	return _localClock.SinceStart();
}
}
