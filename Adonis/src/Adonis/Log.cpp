#include "Log.h"

namespace Adonis {

	std::shared_ptr<spdlog::logger> Log::m_core_logger;
	std::shared_ptr<spdlog::logger> Log::m_client_logger;

	void Log::init() {
		spdlog::set_pattern("%^[%T.%e][%l] %n: %v %$");
		m_core_logger = spdlog::stdout_color_mt("ADONIS");
		m_core_logger->set_level(spdlog::level::trace);

		m_client_logger = spdlog::stdout_color_mt("APP");
		m_core_logger->set_level(spdlog::level::trace);
	}

}