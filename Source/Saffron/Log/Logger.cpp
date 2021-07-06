#include "SaffronPCH.h"

#include "Saffron/Log/Logger.h"

namespace Se
{
Logger::Logger(Shared<spdlog::logger> logger) :
	_nativeLogger(Move(logger))
{
	auto& sinks = const_cast<List<spdlog::sink_ptr>&>(_nativeLogger->sinks());
	sinks.push_back(std::reinterpret_pointer_cast<spdlog::sinks::sink>(CreateShared<InternalSink>(*this)));
}

auto Logger::ToSpdlogLevel(LogLevel logLevel) -> spdlog::level::level_enum
{
	switch (logLevel)
	{
	case LogLevel::Trace: return spdlog::level::trace;
	case LogLevel::Debug: return spdlog::level::debug;
	case LogLevel::Info: return spdlog::level::info;
	case LogLevel::Warn: return spdlog::level::warn;
	case LogLevel::Error: return spdlog::level::err;
	case LogLevel::Critical: return spdlog::level::critical;
	case LogLevel::Off: return spdlog::level::off;
	default: return spdlog::level::off;
	}
}
}
