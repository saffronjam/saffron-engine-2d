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
	explicit Batch(std::string name);
	~Batch();

	void Submit(std::function<void()> function, std::string shortDescription);
	void Execute();
	void ForceExit();
	void Reset();

	auto Progress() const -> float;
	auto JobStatus() const -> const std::string&;
	auto JobCount() const -> size_t;
	auto JobsDone() const -> size_t;
	auto JobsLeft() const -> size_t;

	auto Status() const -> BatchStatus;

	void SetFinalizingStatus(std::string statusMessage);

	auto ExecutionMutex() -> std::mutex&;

public:
	SubscriberList<void> OnStarted;
	SubscriberList<void> OnFinished;

private:
	std::string _name;
	std::atomic<BatchStatus> _status = BatchStatus::Preparing;

	std::vector<std::pair<std::function<void()>, std::string>> _queue;
	std::atomic<size_t> _finishedJobs = 0;
	const std::string _fallback = "";
	std::string _finalizingStatus = "Finalizing";

	std::atomic<float> _progress = 0.0f;
	std::atomic<const std::string*> _jobStatus = nullptr;
	std::mutex _queueMutex, _executionMutex;
	std::thread _worker;
};
}
