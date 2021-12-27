#include "SaffronPCH.h"

#include "Saffron/Log/LogManager.h"

namespace Se
{
auto LogManager::CreateLogger(const std::string& name) -> std::shared_ptr<Logger>
{
	std::ostringstream oss;
	oss << "%^[%T] [%l] " << "%n: " << Log::Fmt::Reset << "%v%$";	

	spdlog::set_pattern(oss.str());
	auto logger = spdlog::stdout_color_mt(name);
	logger->set_level(spdlog::level::trace);

	auto saffronLogger = std::make_shared<Logger>(std::move(logger));
	return saffronLogger;
}
}
