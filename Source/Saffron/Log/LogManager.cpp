#include "SaffronPCH.h"

#include "Saffron/Log/LogManager.h"

namespace Se
{
auto LogManager::CreateLogger(const String& name) -> Shared<Logger>
{
	OStringStream oss;
	oss << "%^[%T] [%l] " << "%n: " << Log::Fmt::Reset << "%v%$";	

	spdlog::set_pattern(oss.str());
	auto logger = spdlog::stdout_color_mt(name);
	logger->set_level(spdlog::level::trace);

	auto saffronLogger = CreateShared<Logger>(Move(logger));
	return saffronLogger;
}
}
