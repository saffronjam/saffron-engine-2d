#pragma once

// Until <format> has been implemented
#include <spdlog/spdlog.h>

#include "Saffron/Core/EventSubscriberList.h"
#include "Saffron/Log/LogLevel.h"
#include "Saffron/Log/LogMessage.h"

namespace Se
{
class Logger
{
public:
	explicit Logger(Shared<spdlog::logger> logger);

	template <typename Arg1, typename... Args>
	void Info(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Post(LogLevel::Info, fmt, arg1, args...);
	}

	template <typename T>
	void Info(const T& msg)
	{
		Post(LogLevel::Info, msg);
	}

	template <typename Arg1, typename... Args>
	void Debug(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Post(LogLevel::Debug, fmt, arg1, args...);
	}

	template <typename T>
	void Debug(const T& msg)
	{
		Post(LogLevel::Debug, msg);
	}

	template <typename Arg1, typename... Args>
	void Warn(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Post(LogLevel::Warn, fmt, arg1, args...);
	}

	template <typename T>
	void Warn(const T& msg)
	{
		Post(LogLevel::Warn, msg);
	}

	template <typename Arg1, typename... Args>
	void Error(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Post(LogLevel::Error, fmt, arg1, args...);
	}

	template <typename T>
	void Error(const T& msg)
	{
		Post(LogLevel::Error, msg);
	}

	template <typename Arg1, typename... Args>
	void Post(LogLevel level, const char* fmt, const Arg1& arg1, const Args&... args)
	{
		_nativeLogger->log(ToSpdlogLevel(level), fmt, arg1, args...);
	}

	template <typename T>
	void Post(LogLevel level, const T& msg)
	{
		_nativeLogger->log(ToSpdlogLevel(level), msg);
	}

public:
	EventSubscriberList<const LogMessage&> OnAll;
	EventSubscriberList<const LogMessage&> OnInfo;
	EventSubscriberList<const LogMessage&> OnDebug;
	EventSubscriberList<const LogMessage&> OnWarn;
	EventSubscriberList<const LogMessage&> OnError;

private:

	class InternalSink : public spdlog::sinks::sink
	{
	public:
		InternalSink(Logger& parent) :
			_parent(parent)
		{
		}

		void log(const spdlog::details::log_msg& msg) override
		{
			const auto logLevel = static_cast<LogLevel>(msg.level);
			const LogMessage logMessage(msg);
			switch (logLevel)
			{
			case LogLevel::Trace: break;
			case LogLevel::Debug:
			{
				_parent.OnDebug.Invoke(logMessage);
				break;
			}
			case LogLevel::Info:
			{
				_parent.OnInfo.Invoke(logMessage);
				break;
			}
			case LogLevel::Warn:
			{
				_parent.OnWarn.Invoke(logMessage);
				break;
			}
			case LogLevel::Error:
			{
				_parent.OnError.Invoke(logMessage);
				break;
			}
			case LogLevel::Critical: break;
			case LogLevel::Off: break;
			default: break;
			}
			_parent.OnAll.Invoke(logMessage);
		}

		void flush() override
		{
		}

	private:
		Logger& _parent;
	};

private:
	auto ToSpdlogLevel(LogLevel logLevel) -> spdlog::level::level_enum;

private:
	std::shared_ptr<spdlog::logger> _nativeLogger;
};
}
