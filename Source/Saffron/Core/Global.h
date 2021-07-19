#pragma once

#include "Saffron/Core/Clock.h"

namespace Se
{
class Global
{
public:
	class Clock
	{
	public:
		using LocalClock = Se::Clock;

		static auto FrameTime() -> sf::Time;
		static auto ElapsedTime() -> sf::Time;
		static auto Restart() -> sf::Time;
		static auto SinceStart()->sf::Time;

	private:
		static LocalClock _localClock;
	};
};
}
