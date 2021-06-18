#pragma once

#include <memory>

namespace Se
{
class Logger;

class LogManager 
{
public:
	static auto CreateLogger(const String& name) -> Shared<Logger>;
};
}
