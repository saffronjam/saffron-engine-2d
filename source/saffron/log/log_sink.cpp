#include "saffron_pch.h"

#include "saffron/log/log_sink.h"

namespace saffron
{
void LogSink::SetLevel(LogLevel level)
{
	set_level(static_cast<spdlog::level::level_enum>(level));
}
}
