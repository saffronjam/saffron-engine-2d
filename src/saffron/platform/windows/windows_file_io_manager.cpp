#include "saffron_pch.h"

#ifdef SE_PLATFORM_WINDOWS

#include <commdlg.h>

#include "saffron/core/filesystem.h"

namespace saffron
{
auto FormatFilter(const Filesystem::Filter& filter) -> const char*
{
	static char buffer[1000] = {0};
	char* bufferP = buffer;

	sprintf_s(bufferP, 1000 - (bufferP - buffer), "%s", filter.Description.c_str());
	bufferP += filter.Description.size() + 1;

	for (const auto& extension : filter.Extensions)
	{
		sprintf_s(bufferP, 1000 - (bufferP - buffer), "%s", extension.c_str());
		bufferP += extension.size() + 1;
	}
	sprintf_s(bufferP, 1000 - (bufferP - buffer), "\0");

	return buffer;
}

auto Filesystem::TryOpenFile(const Filter& filter) -> Result<std::filesystem::path>
{
	const char* formattedFilter = FormatFilter(filter);

	OPENFILENAMEA ofn; // common dialog box structure
	CHAR szFile[260] = {0}; // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = Instance()._window.NativeWindow().getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof szFile;
	ofn.lpstrFilter = formattedFilter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		return ofn.lpstrFile;
	}
	return std::unexpected(Error{ErrorCode::Platform, "Open file dialog was cancelled or failed"});
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
	const char* formattedFilter = FormatFilter(filter);
	const char* fallbackExtension = filter.Extensions.front().c_str();
	if (filter.Extensions.front() == "*.*")
	{
		fallbackExtension = "";
	}

	OPENFILENAMEA ofn; // common dialog box structure
	CHAR szFile[260] = {0}; // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = Instance()._window.NativeWindow().getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.lpstrDefExt = fallbackExtension;
	ofn.nMaxFile = sizeof szFile;
	ofn.lpstrFilter = formattedFilter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT;

	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		return ofn.lpstrFile;
	}
	return std::unexpected(Error{ErrorCode::Platform, "Save file dialog was cancelled or failed"});
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
