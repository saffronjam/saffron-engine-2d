#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Saffron/Core/TypeDefs.h"
#include "Saffron/Log/Logger.h"
#include "Saffron/Math/SaffronMath.h"

namespace Se
{
class Log
{
public:
	Log();

	template <typename Arg1, typename... Args>
	static void Info(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Instance()._clientLogger->Info(fmt, arg1, args...);
	}

	template <typename T>
	static void Info(const T& msg)
	{
		Instance()._clientLogger->Info(msg);
	}

	template <typename Arg1, typename... Args>
	static void Debug(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Instance()._coreLogger->Debug(fmt, arg1, args...);
	}

	template <typename T>
	static void Debug(const T& msg)
	{
		Instance()._coreLogger->Debug(msg);
	}

	template <typename Arg1, typename... Args>
	static void Warn(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Instance()._clientLogger->Warn(fmt, arg1, args...);
	}

	template <typename T>
	static void Warn(const T& msg)
	{
		Instance()._clientLogger->Warn(msg);
	}

	template <typename Arg1, typename... Args>
	static void Error(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Instance()._clientLogger->Error(fmt, arg1, args...);
	}

	template <typename T>
	static void Error(const T& msg)
	{
		Instance()._clientLogger->Error(msg);
	}

	template <typename Arg1, typename... Args>
	static void CoreInfo(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Instance()._coreLogger->Info(fmt, arg1, args...);
	}

	template <typename T>
	static void CoreInfo(const T& msg)
	{
		Instance()._coreLogger->Info(msg);
	}

	template <typename Arg1, typename... Args>
	static void CoreDebug(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Instance()._coreLogger->Debug(fmt, arg1, args...);
	}

	template <typename T>
	static void CoreDebug(const T& msg)
	{
		Instance()._coreLogger->Debug(msg);
	}

	template <typename Arg1, typename... Args>
	static void CoreWarn(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Instance()._coreLogger->Warn(fmt, arg1, args...);
	}

	template <typename T>
	static void CoreWarn(const T& msg)
	{
		Instance()._coreLogger->Warn(msg);
	}

	template <typename Arg1, typename... Args>
	static void CoreError(const char* fmt, const Arg1& arg1, const Args&... args)
	{
		Instance()._coreLogger->Error(fmt, arg1, args...);
	}

	template <typename T>
	static void CoreError(const T& msg)
	{
		Instance()._coreLogger->Error(msg);
	}

	static auto CoreLogger() -> Shared<Logger>;
	static auto ClientLogger() -> Shared<Logger>;

private:
	static auto Instance() -> Log&;

private:
	static Log* _instance;

private:
	Shared<Logger> _coreLogger;
	Shared<Logger> _clientLogger;
};
}

template <typename OStream, typename t_Number>
auto operator<<(OStream& os, const sf::Vector2<t_Number>& vec) -> OStream&
{
	return os << '(' << vec.x << ", " << vec.y << ')';
}

template <typename OStream, typename t_Number>
auto operator<<(OStream& os, const sf::Vector3<t_Number>& vec) -> OStream&
{
	return os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
}
