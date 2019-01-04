#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace Adonis {

	class ADONIS_API Log {

	public:
		
		static void init();

		inline static std::shared_ptr<spdlog::logger>& core_logger() { return m_core_logger; };
		inline static std::shared_ptr<spdlog::logger>& client_logger() { return m_client_logger; };
	private:
		static std::shared_ptr<spdlog::logger> m_core_logger;
		static std::shared_ptr<spdlog::logger> m_client_logger;
	};

}

#define AD_CORE_TRACE(...)		Adonis::Log::core_logger()->trace(__VA_ARGS__)
#define AD_CORE_INFO(...)		Adonis::Log::core_logger()->info(__VA_ARGS__)
#define AD_CORE_WARN(...)		Adonis::Log::core_logger()->warn(__VA_ARGS__)
#define AD_CORE_CRITICAL(...)	Adonis::Log::core_logger()->critical(__VA_ARGS__)
#define AD_CORE_FATAL(...)		Adonis::Log::core_logger()->fatal(__VA_ARGS__)
#define AD_CORE_ERROR(...)		Adonis::Log::core_logger()->error(__VA_ARGS__)

#define AD_TRACE(...)			Adonis::Log::client_logger()->trace(__VA_ARGS__)
#define AD_INFO(...)			Adonis::Log::client_logger()->info(__VA_ARGS__)
#define AD_WARN(...)			Adonis::Log::client_logger()->warn(__VA_ARGS__)
#define AD_CRITICAL(...)		Adonis::Log::client_logger()->critical(__VA_ARGS__)
#define AD_FATAL(...)			Adonis::Log::client_logger()->fatal(__VA_ARGS__)
#define AD_ERROR(...)			Adonis::Log::client_logger()->error(__VA_ARGS__)