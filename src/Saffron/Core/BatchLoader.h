#pragma once

#include "Saffron/Base.h"

namespace Se
{
class BatchLoader
{
public:
	explicit BatchLoader(String name);
	~BatchLoader();

	void Submit(Function<void()> function, String shortDescription);
	void Execute();
	void ForceExit();
	void Reset();

	auto Progress() const -> float;
	auto Status() const -> const String*;
	auto JobCount() const -> size_t;
	auto JobsDone() const -> size_t;
	auto JobsLeft() const -> size_t;

	auto IsFinished() const -> bool;

	auto ExecutionMutex() -> Mutex&;

public:
	EventSubscriberList<void> OnStarted;
	EventSubscriberList<void> OnFinished;

private:
	String _name;

	List<Pair<Function<void()>, String>> _queue;
	size_t _noJobsDone = 0;

	Atomic<float> _progress = 0.0f;
	Atomic<const String*> _status = nullptr;
	Atomic<bool> _running = false, _shouldExit = false;
	Mutex _queueMutex, _executionMutex;
	Thread _worker;
};
}
