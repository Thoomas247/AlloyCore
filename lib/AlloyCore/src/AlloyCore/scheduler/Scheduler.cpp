#include "AlloyCore/scheduler/Scheduler.hpp"

#include "AlloyCore/app/AppState.hpp"

namespace Alloy::Internal
{
	/* -- PUBLIC -- */

	void Scheduler::Run(ScheduleID scheduleID, AppState& appState)
	{
		m_Schedules[(size_t)scheduleID].Run(appState);
	}

	void Scheduler::AddStage(ScheduleID scheduleID, std::string_view stageName)
	{
		m_Schedules[(size_t)scheduleID].AddStage(stageName);
	}

	void Scheduler::AddStageAfter(ScheduleID scheduleID, std::string_view stageName, std::string_view after)
	{
		m_Schedules[(size_t)scheduleID].AddStageAfter(stageName, after);
	}

	void Scheduler::AddStageBefore(ScheduleID scheduleID, std::string_view stageName, std::string_view before)
	{
		m_Schedules[(size_t)scheduleID].AddStageBefore(stageName, before);
	}
}