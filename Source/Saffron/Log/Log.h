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
	struct Fmt
	{
#pragma region Misc
		//@{
		/** Misc */
		inline static const std::string Reset = "\033[m";
		inline static const std::string Bold = "\033[1m";
		inline static const std::string Dark = "\033[2m";
		inline static const std::string Underline = "\033[4m";
		inline static const std::string Blink = "\033[5m";
		inline static const std::string Reverse = "\033[7m";
		inline static const std::string Concealed = "\033[8m";
		inline static const std::string ClearLine = "\033[K";
		//@}
#pragma endregion

#pragma region Foreground
		//@{
		/** Foreground colors */
		inline static const std::string Black = "\033[30m";
		inline static const std::string Red = "\033[31m";
		inline static const std::string Green = "\033[32m";
		inline static const std::string Yellow = "\033[33m";
		inline static const std::string Blue = "\033[34m";
		inline static const std::string Magenta = "\033[35m";
		inline static const std::string Cyan = "\033[36m";
		inline static const std::string White = "\033[37m";
		inline static const std::string BrightBlack = "\u001b[30;1m";
		inline static const std::string BrightRed = "\u001b[31;1m";
		inline static const std::string BrightGreen = "\u001b[32;1m";
		inline static const std::string BrightYellow = " \u001b[33;1m";
		inline static const std::string BrightBlue = "\u001b[34;1m";
		inline static const std::string BrightMagenta = "\u001b[35;1m";
		inline static const std::string BrightCyan = "\u001b[36;1m";
		inline static const std::string BrightWhite = "\u001b[37;1m";
		//@}
#pragma endregion

#pragma region Background
		//@{
		/** Background colors */
		inline static const std::string OnBlack = "\033[40m";
		inline static const std::string OnRed = "\033[41m";
		inline static const std::string OnGreen = "\033[42m";
		inline static const std::string OnYellow = "\033[43m";
		inline static const std::string OnBlue = "\033[44m";
		inline static const std::string OnMagenta = "\033[45m";
		inline static const std::string OnCyan = "\033[46m";
		inline static const std::string OnWhite = "\033[47m";
		//@}
#pragma endregion

#pragma region Bold Colors
		//@{
		/** Bold colors */
		inline static const std::string YellowBold = "\033[33m\033[1m";
		inline static const std::string RedBold = "\033[31m\033[1m";
		inline static const std::string BoldOnRed = "\033[1m\033[41m";
		//@}
#pragma endregion

#pragma region Presets
		inline static const std::string Debug = OnBlack + Cyan;
		inline static const std::string Info = OnBlack + Green;
		inline static const std::string Warn = OnBlack + BrightYellow;
		inline static const std::string Error = OnBlack + Red;
#pragma endregion
	};

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

	static auto CoreLogger() -> std::shared_ptr<Logger>;
	static auto ClientLogger() -> std::shared_ptr<Logger>;

private:
	static auto Instance() -> Log&;

private:
	static Log* _instance;

private:
	std::shared_ptr<Logger> _coreLogger;
	std::shared_ptr<Logger> _clientLogger;
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
