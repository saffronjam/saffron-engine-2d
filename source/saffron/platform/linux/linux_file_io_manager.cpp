#include "saffron_pch.h"

#ifdef SE_PLATFORM_LINUX

#include <array>
#include <cstdio>
#include <string>

#include "saffron/core/filesystem.h"

namespace saffron
{
// Build a "--file-filter" argument for zenity from the engine's filter.
// e.g. Description "Scene (*.sc)" with extensions { "*.sc" } -> "Scene | *.sc"
static auto FormatFilter(const Filesystem::Filter& filter) -> std::string
{
	if (filter.Extensions.empty())
	{
		return {};
	}

	std::string pattern;
	for (const auto& extension : filter.Extensions)
	{
		if (!pattern.empty())
		{
			pattern += ' ';
		}
		pattern += extension;
	}

	return "--file-filter=\"" + filter.Description + " | " + pattern + "\"";
}

// Run a zenity command and return the single path it prints on stdout.
static auto RunZenity(const std::string& command) -> Result<std::filesystem::path>
{
	std::array<char, 1024> buffer{};
	std::string result;

	FILE* pipe = popen(command.c_str(), "r");
	if (pipe == nullptr)
	{
		return std::unexpected(Error{ErrorCode::Platform, "Failed to start zenity file dialog"});
	}

	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr)
	{
		result += buffer.data();
	}
	pclose(pipe);

	// zenity terminates the selection with a newline
	while (!result.empty() && (result.back() == '\n' || result.back() == '\r'))
	{
		result.pop_back();
	}

	if (result.empty())
	{
		return std::unexpected(Error{ErrorCode::Platform, "File dialog was cancelled"});
	}
	return std::filesystem::path(result);
}

auto Filesystem::TryOpenFile(const Filter& filter) -> Result<std::filesystem::path>
{
	const std::string command = "zenity --file-selection " + FormatFilter(filter) + " 2>/dev/null";
	return RunZenity(command);
}

auto Filesystem::OpenFile(const Filter& filter) -> std::filesystem::path
{
	auto result = TryOpenFile(filter);
	if (!result)
	{
		Log::CoreWarn(result.error().message);
		return {};
	}
	return *result;
}

auto Filesystem::TrySaveFile(const Filter& filter) -> Result<std::filesystem::path>
{
	const std::string command =
		"zenity --file-selection --save --confirm-overwrite " + FormatFilter(filter) + " 2>/dev/null";
	return RunZenity(command);
}

auto Filesystem::SaveFile(const Filter& filter) -> std::filesystem::path
{
	auto result = TrySaveFile(filter);
	if (!result)
	{
		Log::CoreWarn(result.error().message);
		return {};
	}
	return *result;
}
}

#endif
