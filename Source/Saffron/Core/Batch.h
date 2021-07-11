#pragma once

#include "Saffron/Base.h"

namespace Se
{
enum class BatchStatus
{
	Preparing,
	Executing,
	Finished
};

class Batch
{
public:
	explicit Batch(String name);
	~Batch();

	void Submit(Function<void()> function, String shortDescription);
	void Execute();
	void ForceExit();
	void Reset();

	auto Progress() const -> float;
	auto JobStatus() const -> const String&;
	auto JobCount() const -> size_t;
	auto JobsDone() const -> size_t;
	auto JobsLeft() const -> size_t;

	auto Status() const -> BatchStatus;

	void SetFinalizingStatus(String statusMessage);

	auto ExecutionMutex() -> Mutex&;

public:
	EventSubscriberList<void> OnStarted;
	EventSubscriberList<void> OnFinished;

private:
	String _name;
	Atomic<BatchStatus> _status = BatchStatus::Preparing;

	List<Pair<Function<void()>, String>> _queue;
	Atomic<size_t> _finishedJobs = 0;
	const String _fallback = "";
	String _finalizingStatus = "Finalizing";

	Atomic<float> _progress = 0.0f;
	Atomic<const String*> _jobStatus = nullptr;
	Mutex _queueMutex, _executionMutex;
	Thread _worker;
};
}
