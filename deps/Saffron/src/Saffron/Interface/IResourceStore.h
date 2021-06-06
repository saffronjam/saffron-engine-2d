#pragma once

#include <map>

#include "Saffron/Interface/IException.h"

namespace Se
{
template <class T>
class IResourceStore
{
public:
	IResourceStore() = default;

protected:
	static std::map<std::string, T> _resources;

public:
	class Exception : public IException
	{
	public:
		Exception(int line, const char* file, const char* errorString) :
			IException(line, file),
			_errorString(errorString)
		{
		}

		auto what() const noexcept -> const char* override
		{
			std::ostringstream oss;
			oss << "[Type] " << GetType() << std::endl << "[Description] " << GetErrorString() << std::endl <<
				GetOriginString();
			whatBuffer = oss.str();
			return whatBuffer.c_str();
		}

		auto GetType() const -> const char* override
		{
			static char buffer[100];
			snprintf(buffer, 100, "%s %s", typeid(T).name(), "Exception");
			return buffer;
		}

		auto GetErrorString() const -> const char*
		{
			return _errorString.c_str();
		}

	private:
		std::string _errorString;
	};
};

template <typename T>
std::map<std::string, T> IResourceStore<T>::_resources;
}
