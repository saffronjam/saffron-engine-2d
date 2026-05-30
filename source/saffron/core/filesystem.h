#pragma once

#include <filesystem>

#include "saffron/base.h"
#include "saffron/core/wrapper_buffer.h"
#include "saffron/core/window.h"

namespace saffron
{
namespace fs = std::filesystem;

class Filesystem : public Singleton<Filesystem>
{
public:
	struct Filter
	{
		std::string Description;
		std::vector<std::string> Extensions;

		static auto Empty() -> Filter;

		Filter() = default;

		Filter(std::string description, std::vector<std::string> extensions) :
			Description(std::move(description)),
			Extensions(std::move(extensions))
		{
		}
	};

public:
	explicit Filesystem(const Window& window);

	// Implemented per platform
	static auto TryOpenFile(const Filter& filter) -> Result<std::filesystem::path>;
	static auto TrySaveFile(const Filter& filter) -> Result<std::filesystem::path>;
	static auto OpenFile(const Filter& filter) -> std::filesystem::path;
	static auto SaveFile(const Filter& filter) -> std::filesystem::path;

	static auto TryAllFiles(const std::filesystem::path& directoryPath,
	                        const std::string& extension) -> Result<std::vector<std::filesystem::directory_entry>>;
	static auto AllFiles(const std::filesystem::path& directoryPath,
	                     const std::string& extension) -> std::vector<std::filesystem::directory_entry>;
	static auto TryFileCount(const std::filesystem::path& directoryPath, const std::string& extension) -> Result<size_t>;
	static auto FileCount(const std::filesystem::path& directoryPath, const std::string& extension) -> size_t;

	static auto TryWrite(const uchar* data, size_t size, const std::filesystem::path& filepath,
	                     bool overwrite) -> Result<size_t>;
	static auto Write(const uchar* data, size_t size, const std::filesystem::path& filepath,
	                  bool overwrite) -> size_t;
	static auto TryWrite(WrapperBuffer buffer, const std::filesystem::path& filepath, bool overwrite) -> Result<size_t>;
	static auto Write(WrapperBuffer buffer, const std::filesystem::path& filepath, bool overwrite) -> size_t;

	static auto TryCreateDirectories(const std::filesystem::path& filepath) -> Status;
	static auto CreateDirectories(const std::filesystem::path& filepath) -> bool;

	static auto TryFileExists(const std::filesystem::path& filepath) -> Result<bool>;
	static auto FileExists(const std::filesystem::path& filepath) -> bool;

	static auto TryCopy(const std::filesystem::path& source, const std::filesystem::path& destination) -> Status;
	static auto Copy(const std::filesystem::path& source, const std::filesystem::path& destination) -> bool;

private:
	const Window& _window;
};
}
