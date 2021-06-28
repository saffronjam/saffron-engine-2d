#include "SaffronPCH.h"

#include "Saffron/Core/Filesystem.h"

namespace Se
{
auto Filesystem::Filter::Empty() -> Filesystem::Filter
{
	return Filter("All Files", {"*.*"});
}

Filesystem::Filesystem(const Window& window) :
	SingleTon(this),
	_window(window)
{
}

auto Filesystem::AllFiles(const Path& directoryPath, const String& extension) -> List<DirEntry>
{
	List<DirEntry> output;

	try
	{
		std::copy_if(fs::directory_iterator(directoryPath), fs::directory_iterator{}, std::back_inserter(output),
		             [&](const DirEntry& entry)
		             {
			             return entry.path().extension() == extension;
		             });
	}
	catch (fs::filesystem_error& fe)
	{
		Log::CoreWarn("Failed to get files from directory: {} with file extension: {}. What: ", directoryPath.string(),
		              extension, fe.what());
	}

	return output;
}

auto Filesystem::FileCount(const Path& directoryPath, const String& extension) -> size_t
{
	// Return early if no extension is given
	if (extension.empty())
	{
		return std::distance(fs::directory_iterator(directoryPath), fs::directory_iterator{});
	}

	try
	{
		return std::count_if(fs::directory_iterator(directoryPath), fs::directory_iterator{}, [&](const DirEntry& entry)
		{
			return entry.path().extension() == extension;
		});
	}
	catch (fs::filesystem_error& fe)
	{
		Log::CoreWarn("Failed to get file count from directory: {} with file extension: {}. What: ",
		              directoryPath.string(), extension, fe.what());
	}
	return 0;
}

auto Filesystem::Write(const uchar* data, size_t size, const Path& filepath, bool overwrite) -> size_t
{
	const bool fileExists = FileExists(filepath);
	if (!fileExists || fileExists && overwrite)
	{
		OFileStream ofstream;
		ofstream.open(filepath);
		if (ofstream.good())
		{
			const auto start = ofstream.tellp();
			ofstream.write(reinterpret_cast<const char*>(data), size);
			return ofstream.tellp() - start;
		}
		Log::CoreWarn("Failed to open file: " + filepath.string());
	}
	return 0;
}

auto Filesystem::Write(const WrapperBuffer buffer, const Path& filepath, bool overwrite) -> size_t
{
	return Write(buffer.Data(), buffer.Size(), filepath, overwrite);
}

auto Filesystem::CreateDirectories(const Path& filepath) -> bool
{
	std::error_code errorCode;
	return create_directories(filepath, errorCode);
}

auto Filesystem::FileExists(const Path& filepath) -> bool
{
	std::error_code errorCode;
	return exists(filepath, errorCode);
}

auto Filesystem::Copy(const Path& source, const Path& destination) -> bool
{
	std::error_code errorCode;
	copy_file(source, destination, errorCode);
	return static_cast<bool>(errorCode);
}
}
