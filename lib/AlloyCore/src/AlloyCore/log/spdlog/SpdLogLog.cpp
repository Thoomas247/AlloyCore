#include "AlloyCore/log/spdlog/SpdLogLog.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Alloy::Internal
{

	/* -- PUBLIC -- */

	SpdLogLog::SpdLogLog()
		: m_Logger(spdlog::stdout_color_mt("ENGINE"))
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");	// color, timestamp, logger name, log message
		m_Logger->set_level(spdlog::level::trace);
	}

	void SpdLogLog::Fatal(const std::string& msg)
	{
		m_Logger->critical(msg);
	}

	void SpdLogLog::Error(const std::string& msg)
	{
		m_Logger->error(msg);
	}

	void SpdLogLog::Warn(const std::string& msg)
	{
		m_Logger->warn(msg);
	}

	void SpdLogLog::Info(const std::string& msg)
	{
		m_Logger->info(msg);
	}

	void SpdLogLog::Trace(const std::string& msg)
	{
		m_Logger->trace(msg);
	}
}