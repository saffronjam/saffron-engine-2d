#pragma once

#include <exception>
#include <sstream>

namespace Se
{
class IException : public std::exception
{
public:
	IException(int line, const char* file);
	auto what() const noexcept -> const char* override;
	virtual auto GetType() const -> const char*;

	auto GetLine() const -> int { return _line; }

	auto GetFile() const -> const std::string& { return _file; }

	auto GetOriginString() const -> std::string;

private:
	int _line;
	std::string _file;

protected:
	mutable std::string whatBuffer;
};
}
