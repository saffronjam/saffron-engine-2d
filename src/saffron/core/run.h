#pragma once

#include <SFML/System/Time.hpp>

#include "saffron/core/singleton.h"
#include "saffron/core/type_defs.h"

namespace saffron
{
class Run : public Singleton<Run>
{
public:
	using Handle = UUID;

private:
	struct PeriodicFunction
	{
		void RunNow();
		void RunNow() const;

		std::function<void()> Function;
		sf::Time Interval;
		sf::Time Counter;
	};
	

	struct AfterFunction
	{
		void RunNow() { Function(); }

		void RunNow() const { Function(); }

		std::function<void()> Function;
		sf::Time Delay;
		sf::Time Counter = sf::Time::Zero;
		bool Executed = false;
	};

public:
	Run();

	void Execute();

	static void Later(std::function<void()> function);
	static void After(std::function<void()> function, sf::Time delay);
	static auto Periodically(std::function<void()> function, sf::Time interval) -> Handle;
	static auto EveryFrame(std::function<void()> function) -> Handle;

	static void Remove(Handle handle);

private:
	std::vector<std::function<void()>> _laterFunctions;
	std::vector<AfterFunction> _afterFunctions;
	std::unordered_map<ulong, PeriodicFunction> _periodicFunctions;
	std::unordered_map<ulong, std::function<void()>> _frameFunctions;
};
}
