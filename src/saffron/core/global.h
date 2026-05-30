#pragma once

#include "saffron/core/clock.h"

namespace saffron
{
class Global
{
public:
	class Clock
	{
	public:
		using LocalClock = saffron::Clock;

		static auto FrameTime() -> sf::Time;
		static auto ElapsedTime() -> sf::Time;
		static auto Restart() -> sf::Time;
		static auto SinceStart()->sf::Time;

	private:
		static LocalClock _localClock;
	};
};
}
