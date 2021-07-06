#include "SaffronPCH.h"

#include "Saffron/Interface/IException.h"

namespace Se
{
IException::IException(int line, const char* file) :
	_line(line),
	_file(file)
{
}

auto IException::what() const noexcept -> const char*
{
	std::ostringstream oss;
	oss << "[Type] " << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

auto IException::GetType() const -> const char*
{
	return "IException";
}

auto IException::GetOriginString() const -> std::string
{
	std::ostringstream oss;
	oss << "[File] " << _file << std::endl << "[Line] " << _line << std::endl;
	return oss.str();
}
}
