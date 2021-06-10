#include "SaffronPCH.h"

#include "Saffron/Core/BatchLoader.h"
#include "Saffron/Core/ScopedLock.h"

namespace Se
{
BatchLoader::BatchLoader(String name) :
	_name(Move(name))
{
}

BatchLoader::~BatchLoader()
{
	ForceExit();
}

void BatchLoader::Submit(Function<void()> function, String shortDescription)
{
	ScopedLock queueLock(_queueMutex);
	_queue.emplace_back(Move(function), Move(shortDescription));
}

void BatchLoader::Execute()
{
	ScopedLock queueLock(_queueMutex);
	if (_worker.joinable())
	{
		_worker.join();
	}

	const auto& workerFn = [this]
	{
		ScopedLock queueLock(_queueMutex);

		_running = true;
		OnStarted.Invoke();

		_progress = 0.0f;
		for (const auto& [function, shortDescription] : _queue)
		{
			if (_shouldExit)
			{
				break;
			}
			ScopedLock scopedLock(_executionMutex);
			_status = &shortDescription;
			function();
			_progress = _progress + 100.0f / static_cast<float>(_queue.size());
		}
		_progress = 100.0f;
		_queue.clear();

		OnFinished.Invoke();
	};

	_worker = Thread(workerFn);
}

void BatchLoader::ForceExit()
{
	_shouldExit = true;
	if (_worker.joinable())
	{
		_worker.join();
	}
}

void BatchLoader::Reset()
{
	ScopedLock queueLock(_queueMutex);
	ScopedLock scopedLock(_executionMutex);
	ForceExit();
	_queue.clear();
	_noJobsDone = 0;
	_progress = 0.0f;
	_shouldExit = false;
}

auto BatchLoader::Progress() const -> float
{
	return _progress;
}

auto BatchLoader::Status() const -> const String*
{
	return _status;
}

auto BatchLoader::JobCount() const -> size_t
{
	return _queue.size();
}

auto BatchLoader::JobsDone() const -> size_t
{
	return _noJobsDone;
}

auto BatchLoader::JobsLeft() const -> size_t
{
	return JobCount() - _noJobsDone;
}

auto BatchLoader::IsFinished() const -> bool
{
	return _progress >= 100.0f;
}

auto BatchLoader::ExecutionMutex() -> Mutex&
{
	return _executionMutex;
}
}
