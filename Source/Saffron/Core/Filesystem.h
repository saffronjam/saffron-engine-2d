#pragma once

#include <filesystem>

#include "Saffron/Base.h"
#include "Saffron/Core/WrapperBuffer.h"
#include "Saffron/Core/Window.h"

namespace Se
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
	static auto OpenFile(const Filter& filter = Filter::Empty()) -> std::filesystem::path;
	static auto SaveFile(const Filter& filter = Filter::Empty()) -> std::filesystem::path;

	static auto AllFiles(const std::filesystem::path& directoryPath,
	                     const std::string& extension = "") -> std::vector<std::filesystem::directory_entry>;
	static auto FileCount(const std::filesystem::path& directoryPath, const std::string& extension = "") -> size_t;

	static auto Write(const uchar* data, size_t size, const std::filesystem::path& filepath,
	                  bool overwrite = true) -> size_t;
	static auto Write(WrapperBuffer buffer, const std::filesystem::path& filepath, bool overwrite = true) -> size_t;

	static auto CreateDirectories(const std::filesystem::path& filepath) -> bool;

	static auto FileExists(const std::filesystem::path& filepath) -> bool;

	static auto Copy(const std::filesystem::path& source, const std::filesystem::path& destination) -> bool;

private:
	const Window& _window;
};
}
