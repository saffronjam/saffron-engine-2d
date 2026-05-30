#include "saffron_pch.h"

#include "saffron/log/log.h"
#include "saffron/log/log_sink.h"
#include "saffron/log/log_manager.h"

namespace saffron
{
Log* Log::_instance = nullptr;

Log::Log()
{
	if (_instance != nullptr)
	{
		std::cerr << typeid(Log).name() << " was already instantiated\n";
		std::abort();
	}
	_instance = this;

	_coreLogger = LogManager::CreateLogger("Saffron");
	_clientLogger = LogManager::CreateLogger("Client");
}

auto Log::CoreLogger() -> std::shared_ptr<Logger>
{
	return Instance()._coreLogger;
}

auto Log::ClientLogger() -> std::shared_ptr<Logger>
{
	return Instance()._clientLogger;
}

auto Log::Instance() -> Log&
{
	if (_instance == nullptr)
	{
		std::cerr << typeid(Log).name() << " was not instantiated\n";
		std::abort();
	}
	return *_instance;
}
}
