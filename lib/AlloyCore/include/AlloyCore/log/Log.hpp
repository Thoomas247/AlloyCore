#pragma once
#include "AlloyCore/standard.hpp"

namespace Alloy::Internal
{
	// forward declarations
	class SpdLogLog;
}

namespace Alloy
{
	/// <summary>
	/// Provides logging capabilities.
	/// </summary>
	class Log
	{
	public:
		static void Fatal(const std::string& msg);
		static void Error(const std::string& msg);
		static void Warn(const std::string& msg);
		static void Info(const std::string& msg);
		static void Trace(const std::string& msg);

	private:
		static std::unique_ptr<Internal::SpdLogLog> s_Instance;
	};

#ifndef AL_DIST
	/// <summary>
	/// Debug-only assertion.
	/// Stripped in distribution builds.
	/// </summary>
	template <typename... Args>
	void ASSERT(bool condition, std::string_view format, Args... args)
	{
		if (!(condition))
		{
			Alloy::Log::Fatal(std::vformat(format, std::make_format_args(args...)));
			__debugbreak();
		}
	}

#undef ERROR	// undefine standard ERROR macro, as it conflicts with the ERROR function below

	/// <summary>
	/// Debug-only error logging.
	/// Stripped in distribution builds.
	/// Use this instead of Log::Error, as the latter is not stripped in distribution builds.
	/// </summary>
	template <typename... Args>
	void ERROR(std::string_view format, Args... args)
	{
		Alloy::Log::Error(std::vformat(format, std::make_format_args(args...)));
	}

	/// <summary>
	/// Debug-only warning logging.
	/// Stripped in distribution builds.
	/// Use this instead of Log::Warn, as the latter is not stripped in distribution builds.
	/// </summary>
	template <typename... Args>
	void WARN(std::string_view format, Args... args)
	{
		Alloy::Log::Warn(std::vformat(format, std::make_format_args(args...)));
	}

	/// <summary>
	/// Debug-only info logging.
	/// Stripped in distribution builds.
	/// Use this instead of Log::Info, as the latter is not stripped in distribution builds.
	/// </summary>
	template <typename... Args>
	void INFO(std::string_view format, Args... args)
	{
		Alloy::Log::Info(std::vformat(format, std::make_format_args(args...)));
	}

	/// <summary>
	/// Debug-only trace logging.
	/// Stripped in distribution builds.
	/// Use this instead of Log::Trace, as the latter is not stripped in distribution builds.
	/// </summary>
	template <typename... Args>
	void TRACE(std::string_view format, Args... args)
	{
		Alloy::Log::Trace(std::vformat(format, std::make_format_args(args...)));
	}
#else
	template <typename... Args>
	constexpr void ASSERT(bool condition, std::string_view format, Args... args) {}

	template <typename... Args>
	constexpr void ERROR(std::string_view format, Args... args) {}

	template <typename... Args>
	constexpr void WARN(std::string_view format, Args... args) {}

	template <typename... Args>
	constexpr void INFO(std::string_view format, Args... args) {}

	template <typename... Args>
	constexpr void TRACE(std::string_view format, Args... args) {}
#endif // !AL_DIST

}
