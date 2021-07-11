#include "SaffronPCH.h"

#include "Saffron/Core/Batch.h"

namespace Se
{
Batch::Batch(String name) :
	_name(Move(name))
{
}

Batch::~Batch()
{
	ForceExit();
}

void Batch::Submit(Function<void()> function, String shortDescription)
{
	if (_status != BatchStatus::Preparing)
	{
		Debug::Break("Batch must be in preparing state when submitting jobs");
		return;
	}

	std::scoped_lock queueLock(_queueMutex);
	_queue.emplace_back(Move(function), Move(shortDescription));
}

void Batch::Execute()
{
	std::scoped_lock queueLock(_queueMutex);
	if (_worker.joinable())
	{
		_worker.join();
	}

	const auto& workerFn = [this]
	{
		std::scoped_lock queueLock(_queueMutex);

		_status = BatchStatus::Executing;
		OnStarted.Invoke();

		_progress = 0.0f;
		for (const auto& [function, shortDescription] : _queue)
		{
			// If status has changed to finished, a request to exit has been made
			if (_status == BatchStatus::Finished)
			{
				break;
			}
			std::scoped_lock scopedLock(_executionMutex);
			_jobStatus = &shortDescription;
			function();
			_finishedJobs = _finishedJobs + 1;
			_progress = _progress + 100.0f / static_cast<float>(_queue.size());
		}
		_jobStatus = &_finalizingStatus;
		_status = BatchStatus::Finished;
		_progress = 100.0f;

		OnFinished.Invoke();
	};

	_worker = Thread(workerFn);
}

void Batch::ForceExit()
{
	_status = BatchStatus::Finished;
	if (_worker.joinable())
	{
		_worker.join();
	}
}

void Batch::Reset()
{
	std::scoped_lock queueLock(_queueMutex);
	std::scoped_lock scopedLock(_executionMutex);
	ForceExit();
	_queue.clear();
	_finishedJobs = 0;
	_progress = 0.0f;
	_status = BatchStatus::Preparing;
}

auto Batch::Progress() const -> float
{
	return _progress;
}

auto Batch::JobStatus() const -> const String&
{
	return _jobStatus == nullptr ? _fallback : *_jobStatus;
}

auto Batch::JobCount() const -> size_t
{
	return _queue.size();
}

auto Batch::JobsDone() const -> size_t
{
	return _finishedJobs;
}

auto Batch::JobsLeft() const -> size_t
{
	return JobCount() - _finishedJobs;
}

auto Batch::Status() const -> BatchStatus
{
	return _status;
}

void Batch::SetFinalizingStatus(String statusMessage)
{
	_finalizingStatus = statusMessage;
}

auto Batch::ExecutionMutex() -> Mutex&
{
	return _executionMutex;
}
}
