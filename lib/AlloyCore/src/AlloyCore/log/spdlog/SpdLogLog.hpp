#pragma once
#include "AlloyCore/standard.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Alloy::Internal
{
	/// <summary>
	/// Wrapper around a spdlog logger.
	/// Used to initialize the logger as a static member of the Log class.
	/// </summary>
	class SpdLogLog
	{
	public:
		SpdLogLog()
			: m_Logger(spdlog::stdout_color_mt("ENGINE"))
		{
			spdlog::set_pattern("%^[%T] %n: %v%$");	// color, timestamp, logger name, log message
			m_Logger->set_level(spdlog::level::trace);
		}

		void Fatal(const std::string& msg)
		{
			m_Logger->critical(msg);
		}

		void Error(const std::string& msg)
		{
			m_Logger->error(msg);
		}

		void Warn(const std::string& msg)
		{
			m_Logger->warn(msg);
		}

		void Info(const std::string& msg)
		{
			m_Logger->info(msg);
		}

		void Trace(const std::string& msg)
		{
			m_Logger->trace(msg);
		}

	private:
		std::shared_ptr<spdlog::logger> m_Logger;
	};
}