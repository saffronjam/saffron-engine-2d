#pragma once

#include <filesystem>

#include "Saffron/Base.h"
#include "Saffron/Core/WrapperBuffer.h"
#include "Saffron/Core/Window.h"

namespace Se
{
namespace fs = std::filesystem;

class Filesystem : public SingleTon<Filesystem>
{
public:
	struct Filter
	{
		String Description;
		List<String> Extensions;

		static auto Empty() -> Filter;

		Filter() = default;

		Filter(String description, List<String> extensions) :
			Description(Move(description)),
			Extensions(Move(extensions))
		{
		}
	};

public:
	explicit Filesystem(const Window& window);

	// Implemented per platform
	static auto OpenFile(const Filter& filter = Filter::Empty()) -> Path;
	static auto SaveFile(const Filter& filter = Filter::Empty()) -> Path;

	static auto AllFiles(const Path& directoryPath, const String& extension = "") -> List<DirEntry>;
	static auto FileCount(const Path& directoryPath, const String& extension = "") -> size_t;

	static auto Write(const Uint8* data, size_t size, const Path& filepath, bool overwrite = true) -> size_t;
	static auto Write(WrapperBuffer buffer, const Path& filepath, bool overwrite = true) -> size_t;

	static auto CreateDirectories(const Path& filepath) -> bool;

	static auto FileExists(const Path& filepath) -> bool;

	static auto Copy(const Path& source, const Path& destination) -> bool;

private:
	const Window& _window;
};
}
