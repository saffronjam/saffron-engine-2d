#pragma once

#include <expected>
#include <string>

namespace saffron
{
enum class ErrorCode
{
	Unknown,
	InvalidArgument,
	Io,
	Resource,
	Graphics,
	Platform,
};

struct Error
{
	ErrorCode code = ErrorCode::Unknown;
	std::string message;
};

template <typename T>
using Result = std::expected<T, Error>;

using Status = std::expected<void, Error>;
}
