#pragma once

#include "Saffron/Base.h"

namespace Se
{
class ThreadPool
{
private:
	struct Worker
	{
		String Id;
		Thread Thread;
		Atomic<bool> Available = false;
		Function<void()> Function;
		Atomic<int> UseCounter = 0;
	};

public:
	ThreadPool();
	~ThreadPool();
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) noexcept = delete;
	ThreadPool& operator=(const ThreadPool&) const = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	auto DispatchWork(const String& id, Function<void()> fn) -> bool;

	void CollectAll()
	{
		for (auto& worker : _workers)
		{
			worker.Available = false;
			if (worker.Thread.joinable())
			{
				worker.Thread.join();
			}
			worker.Id.clear();
			worker.Available = true;
		}
	}

	static void ThreadFn(Worker& worker)
	{
		try
		{
			worker.Function();
		}
		catch (const std::exception& e)
		{
			Debug::Break("ThreadPool caught uncaught thread exception: {}", e.what());
		}
		worker.Function = nullptr;
		worker.Id.clear();
		worker.Available = true;
	}

private:
	static constexpr int MaxThreads = 24;

	Mutex _mutex;
	Array<Worker, MaxThreads> _workers;
};
}
