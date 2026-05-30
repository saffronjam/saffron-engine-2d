#pragma once

#include <memory>

namespace saffron
{
class Logger;

class LogManager 
{
public:
	static auto CreateLogger(const std::string& name) -> std::shared_ptr<Logger>;
};
}
