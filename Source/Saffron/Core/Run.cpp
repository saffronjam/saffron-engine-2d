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
	Singleton(this)
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
		periodicFunction.Counter += ts;
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

void Run::Later(std::function<void()> function)
{
	Instance()._laterFunctions.push_back(std::move(function));
}

void Run::After(std::function<void()> function, sf::Time delay)
{
	Instance()._afterFunctions.emplace_back(AfterFunction{std::move(function), delay});
}

Run::Handle Run::Periodically(std::function<void()> function, sf::Time interval)
{
	Handle newHandle;
	Instance()._periodicFunctions.emplace(newHandle, PeriodicFunction{std::move(function), interval, sf::Time::Zero});
	return newHandle;
}

Run::Handle Run::EveryFrame(std::function<void()> function)
{
	Handle newHandle;
	Instance()._frameFunctions.emplace(newHandle, std::move(function));
	return newHandle;
}

void Run::Remove(Handle handle)
{
	Instance()._periodicFunctions.erase(handle);
	Instance()._frameFunctions.erase(handle);
}
}
