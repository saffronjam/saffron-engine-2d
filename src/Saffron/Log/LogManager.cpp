#include "SaffronPCH.h"

#include "Saffron/Log/LogManager.h"

namespace Se
{
auto LogManager::CreateLogger(const String& name) -> Shared<Logger>
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	auto logger = spdlog::stdout_color_mt(name.c_str());
	logger->set_level(spdlog::level::trace);

	auto saffronLogger = CreateShared<Logger>(Move(logger));
	return saffronLogger;
}
}
