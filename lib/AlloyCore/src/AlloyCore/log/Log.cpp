#include "AlloyCore/log/Log.hpp"

#include "AlloyCore/log/spdlog/SpdLogLog.hpp"

namespace Alloy
{
	/* -- PUBLIC -- */

	/*static*/ void Log::Fatal(const std::string& msg)
	{
		s_Instance->Fatal(msg);
	}

	/*static*/ void Log::Error(const std::string& msg)
	{
		s_Instance->Error(msg);
	}

	/*static*/ void Log::Warn(const std::string& msg)
	{
		s_Instance->Warn(msg);
	}

	/*static*/ void Log::Info(const std::string& msg)
	{
		s_Instance->Info(msg);
	}

	/*static*/ void Log::Trace(const std::string& msg)
	{
		s_Instance->Trace(msg);
	}


	/* -- PRIVATE -- */

	/*static*/ std::unique_ptr<Internal::SpdLogLog> Log::s_Instance = std::make_unique<Internal::SpdLogLog>();
}