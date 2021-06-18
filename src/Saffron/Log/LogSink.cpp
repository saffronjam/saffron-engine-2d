#include "SaffronPCH.h"

#include "Saffron/Log/LogSink.h"

namespace Se
{
void LogSink::SetLevel(LogLevel level)
{
	set_level(static_cast<spdlog::level::level_enum>(level));
}
}
