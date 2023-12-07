#pragma once
#include "AlloyCore/standard.hpp"

#include "spdlog/spdlog.h"

namespace Alloy::Internal
{
	/// <summary>
	/// Wrapper around a spdlog logger.
	/// Used to initialize the logger as a static member of the Log class.
	/// </summary>
	class SpdLogLog
	{
	public:
		SpdLogLog();

		void Fatal(const std::string& msg);
		void Error(const std::string& msg);
		void Warn(const std::string& msg);
		void Info(const std::string& msg);
		void Trace(const std::string& msg);

	private:
		std::shared_ptr<spdlog::logger> m_Logger;
	};
}