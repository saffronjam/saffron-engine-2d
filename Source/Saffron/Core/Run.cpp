#include "SaffronPCH.h"

#include "Saffron/Core/Global.h"
#include "Saffron/Core/Run.h"

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

	auto& laterFunctions = Instance()._laterFunctions;
	for (auto& function : laterFunctions)
	{
		function();
	}
	laterFunctions.clear();

	auto& afterFunctions = Instance()._afterFunctions;
	for (auto& afterFunction : afterFunctions)
	{
		afterFunction.Counter += ts;
		if (afterFunction.Counter > afterFunction.Delay)
		{
			afterFunction();
			afterFunction.Executed = true;
		}
	}
	afterFunctions.erase(std::ranges::remove_if(afterFunctions, [](const AfterFunction& af)
	{
		return af.Executed;
	}).begin(), _afterFunctions.end());

	for (auto& periodicFunction : _periodicFunctions | std::views::values)
	{
		periodicFunction.Counter+= ts;
		if (periodicFunction.Counter >= periodicFunction.Interval)
		{
			periodicFunction.Counter = sf::Time::Zero;
			periodicFunction();
		}
	}

	for (auto& frameFunction : _frameFunctions | std::views::values)
	{
		frameFunction();
	}
}

void Run::Later(Function<void()> function)
{
	Instance()._laterFunctions.push_back(Move(function));
}

void Run::After(Function<void()> function, sf::Time delay)
{
	Instance()._afterFunctions.emplace_back(AfterFunction{Move(function), delay});
}

Run::Handle Run::Periodically(Function<void()> function, sf::Time interval)
{
	Handle newHandle;
	Instance()._periodicFunctions.emplace(newHandle, PeriodicFunction{Move(function), interval, sf::Time::Zero});
	return newHandle;
}

Run::Handle Run::EveryFrame(Function<void()> function)
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
