#include "AlloyCore/app/Application.hpp"

namespace Alloy
{
	/* -- PUBLIC -- */

	Application::Application()
	{
	}

	void Application::Run()
	{
		// add default stages
		m_State.Scheduler.AddStage(ScheduleID::Startup, DEFAULT_STARTUP_STAGE);
		m_State.Scheduler.AddStage(ScheduleID::Update, DEFAULT_UPDATE_STAGE);
		m_State.Scheduler.AddStage(ScheduleID::Render, DEFAULT_RENDER_STAGE);
		m_State.Scheduler.AddStage(ScheduleID::Shutdown, DEFAULT_SHUTDOWN_STAGE);

		// add default resources
		m_State.ResourceList.AddResource<AppStatus>();

		// reset the command buffer pool with enough commands for all plugins
		m_State.CommandBufferPool.Reset(m_State.Plugins.size());

		// build plugins
		// TODO: multithread this?
		for (auto& plugin : m_State.Plugins)
		{
			plugin->Build(m_State.CommandBufferPool.CreateCommands(m_State));
		}

		// run the commands
		m_State.CommandBufferPool.RunAll(m_State);

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
}