#include "log.h"
#include <cassert>
#include <spdlog/sinks/stdout_color_sinks.h>

static std::shared_ptr<spdlog::logger> spd_logger;

void Rendy::Log::init_logger()
{
	OPTICK_EVENT();
	spdlog::set_pattern("%^[%T][%t]%v%$");
	spd_logger = spdlog::stdout_color_mt("Rendy");
	spd_logger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Rendy::Log::get_logger()
{
	OPTICK_EVENT();
	assert(spd_logger != nullptr);
	return spd_logger;
}
