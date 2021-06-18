#include "SaffronPCH.h"

#include "Saffron/Log/Log.h"
#include "Saffron/Log/LogSink.h"
#include "Saffron/Log/LogManager.h"

namespace Se
{
Log* Log::_instance = nullptr;

Log::Log()
{
	Debug::Assert(_instance == nullptr, String(typeid(Log).name()) + " was already instansiated");;
	_instance = this;

	_coreLogger = LogManager::CreateLogger("Saffron");
	_clientLogger = LogManager::CreateLogger("Client");
}

auto Log::CoreLogger() -> Shared<Logger>
{
	return Instance()._coreLogger;
}

auto Log::ClientLogger() -> Shared<Logger>
{
	return Instance()._clientLogger;
}

auto Log::Instance() -> Log&
{
	Debug::Assert(_instance != nullptr, String(typeid(Log).name()) + " was not instansiated");;
	return *_instance;
}
}
