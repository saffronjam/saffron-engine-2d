#pragma once

#include "saffron/base.h"
#include "saffron/log/log.h"
#include "saffron/log/log_message.h"

#include <format>

namespace saffron
{
class LogSink : public spdlog::sinks::sink
{
	friend class Log;

public:
	virtual void Sink(const LogMessage& message) = 0;
	virtual void Flush() = 0;

	void SetLevel(LogLevel level);

protected:
	void log(const spdlog::details::log_msg& msg) final override
	{
		std::scoped_lock lock(_mutex);
		Sink(LogMessage(msg));
	}

	void flush() final override
	{
		std::scoped_lock lock(_mutex);
		Flush();
	}

private:
	std::mutex _mutex;
};
}
