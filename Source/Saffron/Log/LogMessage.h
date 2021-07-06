#pragma once

#include <spdlog/details/log_msg.h>

#include "Saffron/Log/LogLevel.h"

namespace Se
{
class LogMessage
{
public:
	explicit LogMessage(const spdlog::details::log_msg& message) :
		Raw(message.raw.c_str()),
		Formatted(message.formatted.c_str()),
		Level(static_cast<LogLevel>(message.level))
	{
	}

	String Raw;
	String Formatted;
	LogLevel Level;
};
}
