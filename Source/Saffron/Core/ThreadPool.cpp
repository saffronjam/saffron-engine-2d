#include "SaffronPCH.h"

#include <SFML/System/Sleep.hpp>

#include "Saffron/Core/ThreadPool.h"

namespace Se
{
ThreadPool::ThreadPool()
{
	for (auto& worker : _workers)
	{
		worker.Available = true;
	}
}

ThreadPool::~ThreadPool()
{
	CollectAll();
}

auto ThreadPool::DispatchWork(const String& id, Function<void()> fn) -> bool
{
	std::scoped_lock scoped(_mutex);

	if (std::ranges::find_if(_workers, [id](const Worker& worker) { return worker.Id == id; }) != _workers.end())
	{
		return false;
	}

	auto foundWorker = false;
	while (!foundWorker)
	{
		for (auto& worker : _workers)
		{
			if (worker.Available)
			{
				if (worker.Thread.joinable())
				{
					worker.Thread.join();
				}

				foundWorker = true;
				worker.Id = id;
				worker.Available = false;
				worker.Function = fn;
				worker.Thread = Thread([&worker]
				{
					ThreadFn(worker);
				});
				worker.UseCounter = worker.UseCounter.load() + 1;
				break;
			}
		}
		if (!foundWorker)
		{
			sf::sleep(sf::seconds(0.05f));
		}
	}
	return true;
}
}
