#pragma once
#include "standard.hpp"

#include "log/Log.hpp"
#include "resource/Resource.hpp"
#include "System.hpp"
#include "Stage.hpp"
#include "Schedule.hpp"

namespace Alloy
{
	/// <summary>
	/// Specifies when a schedule should be executed.
	/// Startup stages are executed before the main loop.
	/// Update stages are executed every frame.
	/// Render stages are executed every frame after the update stages.
	/// Shutdown stages are executed after the main loop.
	/// </summary>
	enum class ScheduleID
	{
		Startup,
		Update,
		Render,
		Shutdown,

		NUM_SCHEDULES
	};
}

namespace Alloy::Internal
{

	/// <summary>
	/// Manages systems and when they are executed.
	/// </summary>
	class Scheduler
	{
	public:
		/// <summary>
		/// Runs the given schedule.
		/// </summary>
		void Run(ScheduleID scheduleID)
		{
			m_Schedules[(size_t)scheduleID].Run();
		}

		/// <summary>
		/// Adds a stage to the end of given schedule.
		/// The new stage must not already exist.
		/// </summary>
		void AddStage(ScheduleID scheduleID, std::string_view stageName)
		{
			m_Schedules[(size_t)scheduleID].AddStage(stageName);
		}

		/// <summary>
		/// Adds a stage to the given schedule after the given stage.
		/// </summary>
		void AddStageAfter(ScheduleID scheduleID, std::string_view stageName, std::string_view after)
		{
			m_Schedules[(size_t)scheduleID].AddStageAfter(stageName, after);
		}

		/// <summary>
		/// Adds a stage to the given schedule before the given stage.
		/// </summary>
		void AddStageBefore(ScheduleID scheduleID, std::string_view stageName, std::string_view before)
		{
			m_Schedules[(size_t)scheduleID].AddStageBefore(stageName, before);
		}

		/// <summary>
		/// Adds a system to the given stage in the given schedule.
		/// Both the stage and the schedule must exist, otherwise an error will occur.
		/// </summary>
		template <typename... Args>
		void AddSystem(AppState& appState, ScheduleID scheduleID, std::string_view stageName, System<Args...> system)
		{
			m_Schedules[(size_t)scheduleID].AddSystem(appState, stageName, system);
		}

	private:
		std::array<Schedule, (size_t)ScheduleID::NUM_SCHEDULES> m_Schedules;
	};

}