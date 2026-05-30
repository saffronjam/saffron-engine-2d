#include "saffron_pch.h"

#include "saffron/core/filesystem.h"

namespace saffron
{
auto Filesystem::Filter::Empty() -> Filesystem::Filter
{
	return Filter("All Files", {"*.*"});
}

Filesystem::Filesystem(const Window& window) :
	Singleton(this),
	_window(window)
{
}

auto Filesystem::TryAllFiles(const std::filesystem::path& directoryPath,
                             const std::string& extension) -> Result<std::vector<std::filesystem::directory_entry>>
{
	std::vector<std::filesystem::directory_entry> output;
	std::error_code errorCode;
	fs::directory_iterator iterator(directoryPath, errorCode);
	if (errorCode)
	{
		return std::unexpected(Error{ErrorCode::Io, "Failed to open directory " + directoryPath.string() + ": " + errorCode.message()});
	}

	for (const auto& entry : iterator)
	{
		if (extension.empty() || entry.path().extension() == extension)
		{
			output.push_back(entry);
		}
	}
	return output;
}

auto Filesystem::AllFiles(const std::filesystem::path& directoryPath,
                          const std::string& extension) -> std::vector<std::filesystem::directory_entry>
{
	auto result = TryAllFiles(directoryPath, extension);
	if (!result)
	{
		Log::CoreWarn(result.error().message);
		return {};
	}
	return *result;
}

auto Filesystem::TryFileCount(const std::filesystem::path& directoryPath, const std::string& extension) -> Result<size_t>
{
	auto files = TryAllFiles(directoryPath, extension);
	if (!files)
	{
		return std::unexpected(files.error());
	}
	return files->size();
}

auto Filesystem::FileCount(const std::filesystem::path& directoryPath, const std::string& extension) -> size_t
{
	auto result = TryFileCount(directoryPath, extension);
	if (!result)
	{
		Log::CoreWarn(result.error().message);
		return 0;
	}
	return *result;
}

auto Filesystem::TryWrite(const uchar* data, size_t size, const std::filesystem::path& filepath, bool overwrite) -> Result<size_t>
{
	auto exists = TryFileExists(filepath);
	if (!exists)
	{
		return std::unexpected(exists.error());
	}
	if (*exists && !overwrite)
	{
		return std::unexpected(Error{ErrorCode::Io, "Refusing to overwrite existing file " + filepath.string()});
	}

	std::ofstream ofstream(filepath, std::ios::binary);
	if (!ofstream.good())
	{
		return std::unexpected(Error{ErrorCode::Io, "Failed to open file " + filepath.string()});
	}

	const auto start = ofstream.tellp();
	ofstream.write(reinterpret_cast<const char*>(data), size);
	if (!ofstream.good())
	{
		return std::unexpected(Error{ErrorCode::Io, "Failed to write file " + filepath.string()});
	}
	return static_cast<size_t>(ofstream.tellp() - start);
}

auto Filesystem::Write(const uchar* data, size_t size, const std::filesystem::path& filepath, bool overwrite) -> size_t
{
	auto result = TryWrite(data, size, filepath, overwrite);
	if (!result)
	{
		Log::CoreWarn(result.error().message);
		return 0;
	}
	return *result;
}

auto Filesystem::TryWrite(const WrapperBuffer buffer, const std::filesystem::path& filepath, bool overwrite) -> Result<size_t>
{
	return TryWrite(buffer.Data(), buffer.Size(), filepath, overwrite);
}

auto Filesystem::Write(const WrapperBuffer buffer, const std::filesystem::path& filepath, bool overwrite) -> size_t
{
	return Write(buffer.Data(), buffer.Size(), filepath, overwrite);
}

auto Filesystem::TryCreateDirectories(const std::filesystem::path& filepath) -> Status
{
	std::error_code errorCode;
	create_directories(filepath, errorCode);
	if (errorCode)
	{
		return std::unexpected(Error{ErrorCode::Io, "Failed to create directories " + filepath.string() + ": " + errorCode.message()});
	}
	return {};
}

auto Filesystem::CreateDirectories(const std::filesystem::path& filepath) -> bool
{
	auto result = TryCreateDirectories(filepath);
	if (!result)
	{
		Log::CoreWarn(result.error().message);
		return false;
	}
	return true;
}

auto Filesystem::TryFileExists(const std::filesystem::path& filepath) -> Result<bool>
{
	std::error_code errorCode;
	const auto result = exists(filepath, errorCode);
	if (errorCode)
	{
		return std::unexpected(Error{ErrorCode::Io, "Failed to inspect file " + filepath.string() + ": " + errorCode.message()});
	}
	return result;
}

auto Filesystem::FileExists(const std::filesystem::path& filepath) -> bool
{
	auto result = TryFileExists(filepath);
	if (!result)
	{
		Log::CoreWarn(result.error().message);
		return false;
	}
	return *result;
}

auto Filesystem::TryCopy(const std::filesystem::path& source, const std::filesystem::path& destination) -> Status
{
	std::error_code errorCode;
	copy_file(source, destination, fs::copy_options::overwrite_existing, errorCode);
	if (errorCode)
	{
		return std::unexpected(Error{ErrorCode::Io, "Failed to copy " + source.string() + " to " + destination.string() + ": " + errorCode.message()});
	}
	return {};
}

auto Filesystem::Copy(const std::filesystem::path& source, const std::filesystem::path& destination) -> bool
{
	auto result = TryCopy(source, destination);
	if (!result)
	{
		Log::CoreWarn(result.error().message);
		return false;
	}
	return true;
}
}
