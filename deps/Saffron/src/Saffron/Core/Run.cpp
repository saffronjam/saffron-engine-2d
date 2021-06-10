#include "SaffronPCH.h"

#include "Saffron/Core/Global.h"
#include "Saffron/Core/Run.h"

#include <ranges>

namespace Se
{
void Run::PeriodicFunction::operator()()
{
	Function();
}

void Run::PeriodicFunction::operator()() const
{
	Function();
}

Run::Run() :
	SingleTon(this)
{
}

void Run::Execute()
{
	const auto ts = Global::Clock::FrameTime();

	for (auto& function : Instance()._laterFunctions)
	{
		function();
	}
	Instance()._laterFunctions.clear();

	for (auto& periodicFunction : Instance()._periodicFunctions | std::views::values)
	{
		periodicFunction.Counter += ts;
		if (periodicFunction.Counter >= periodicFunction.Interval)
		{
			periodicFunction.Counter = sf::Time::Zero;
			periodicFunction();
		}
	}

	for (auto& frameFunction : Instance()._frameFunctions | std::views::values)
	{
		frameFunction();
	}
}

void Run::Later(Function<void()> function)
{
	Instance()._laterFunctions.push_back(Move(function));
}

auto Run::Periodically(Function<void()> function, sf::Time interval) -> Handle
{
	Handle newHandle;
	Instance()._periodicFunctions.emplace(newHandle, PeriodicFunction{Move(function), interval, sf::Time::Zero});
	return newHandle;
}

auto Run::EveryFrame(Function<void()> function) -> Handle
{
	Handle newHandle;
	Instance()._frameFunctions.emplace(newHandle, Move(function));
	return newHandle;
}

void Run::Remove(Handle handle)
{
	Instance()._periodicFunctions.erase(handle);
	Instance()._frameFunctions.erase(handle);
}
}
