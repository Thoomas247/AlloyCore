#include "Application.hpp"

namespace Alloy
{
	/* -- PUBLIC -- */

	Application::Application()
	{
		// add default stages
		AddStage(ScheduleID::Startup, DEFAULT_STARTUP_STAGE);
		AddStage(ScheduleID::Update, DEFAULT_UPDATE_STAGE);
		AddStage(ScheduleID::Render, DEFAULT_RENDER_STAGE);
		AddStage(ScheduleID::Shutdown, DEFAULT_SHUTDOWN_STAGE);

		// add default resources
		AddResource<AppStatus>();
	}

	void Application::Run()
	{
		// build plugins
		for (auto& plugin : m_State.Plugins)
		{
			plugin->Build(*this);
		}

		// run startup schedule
		m_State.Scheduler.Run(ScheduleID::Startup, m_State);

		while (m_State.ResourceList.GetResource<const AppStatus>().Running)
		{
			// run update schedule
			m_State.Scheduler.Run(ScheduleID::Update, m_State);

			// run render schedule
			m_State.Scheduler.Run(ScheduleID::Render, m_State);
		}

		// run shutdown schedule
		m_State.Scheduler.Run(ScheduleID::Shutdown, m_State);
	}

	void Application::AddStage(ScheduleID scheduleID, std::string_view stageName)
	{
		m_State.Scheduler.AddStage(scheduleID, stageName);
	}

	void Application::AddStageAfter(ScheduleID scheduleID, std::string_view stageName, std::string_view after)
	{
		m_State.Scheduler.AddStageAfter(scheduleID, stageName, after);
	}

	void Application::AddStageBefore(ScheduleID scheduleID, std::string_view stageName, std::string_view before)
	{
		m_State.Scheduler.AddStageBefore(scheduleID, stageName, before);
	}
}