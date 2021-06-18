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

public:
	Run();

	void Execute();

	static void Later(Function<void()> function);
	static auto Periodically(Function<void()> function, sf::Time interval) -> Handle;
	static auto EveryFrame(Function<void()> function) -> Handle;

	static void Remove(Handle handle);

private:
	List<Function<void()>> _laterFunctions;
	HashMap<Handle, PeriodicFunction> _periodicFunctions;
	HashMap<Handle, Function<void()>> _frameFunctions;
};
}
