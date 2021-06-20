#pragma once

#include <SFML/System/Time.hpp>

#include "Saffron/Core/SingleTon.h"
#include "Saffron/Core/TypeDefs.h"

namespace Se
{
class Run : public SingleTon<Run>
{
public:
	using Handle = UUID;

private:
	struct PeriodicFunction
	{
		void operator()();
		void operator()() const;

		Function<void()> Function;
		sf::Time Interval;
		sf::Time Counter;
	};
	

	struct AfterFunction
	{
		void operator()() { Function(); }

		void operator()() const { Function(); }

		Function<void()> Function;
		sf::Time Delay;
		sf::Time Counter = sf::Time::Zero;
		bool Executed = false;
	};

public:
	Run();

	void Execute();

	static void Later(Function<void()> function);
	static void After(Function<void()> function, sf::Time delay);
	static auto Periodically(Function<void()> function, sf::Time interval) -> Handle;
	static auto EveryFrame(Function<void()> function) -> Handle;

	static void Remove(Handle handle);

private:
	List<Function<void()>> _laterFunctions;
	List<AfterFunction> _afterFunctions;
	HashMap<Handle, PeriodicFunction> _periodicFunctions;
	HashMap<Handle, Function<void()>> _frameFunctions;
};
}
