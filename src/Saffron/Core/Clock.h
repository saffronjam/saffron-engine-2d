#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace Se
{
class Clock
{
public:
	Clock();

	auto FrameTime() const -> sf::Time;
	auto ElapsedTime() const -> sf::Time;
	auto Restart() -> sf::Time;

private:
	sf::Clock _clock;
	sf::Time _dt;
};
}
