#include "Commands.hpp"

#include "EntityCommands.hpp"

namespace Alloy
{

	/* -- PUBLIC -- */

	Commands::Commands(Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
		: m_CommandBuffer(commandBuffer), m_AppState(appState)
	{
	}

	EntityCommands Commands::SpawnEntity()
	{
		// create a future entity id which will be set when the command is executed
		size_t futureEntityIDIndex = m_CommandBuffer.CreateFutureEntityID();

		// add the command to the list
		// we capture the future entity id index so we can set it when the command is executed
		m_CommandBuffer.AddCommand([futureEntityIDIndex](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				// spawn an entity
				EntityID id = appState.Scene.SpawnEntity();

				// set the future entity id in case a future command needs it
				commandBuffer.SetFutureEntityID(futureEntityIDIndex, id);
			});

		return EntityCommands(m_CommandBuffer, m_AppState, futureEntityIDIndex);
	}

	EntityCommands Commands::GetEntity(EntityID entityID)
	{
		// create a future entity id which already holds the entity id
		size_t futureEntityIDIndex = m_CommandBuffer.CreateFutureEntityID(entityID);

		return EntityCommands(m_CommandBuffer, m_AppState, futureEntityIDIndex);
	}

	void Commands::DestroyEntity(EntityID entityID)
	{
		m_CommandBuffer.AddCommand([entityID](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.Scene.DespawnEntity(entityID);
			});
	}

	void Commands::AddStage(ScheduleID scheduleID, std::string_view stageName)
	{
		m_CommandBuffer.AddCommand([scheduleID, stageName](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.Scheduler.AddStage(scheduleID, stageName);
			});
	}

	void Commands::AddStageAfter(ScheduleID scheduleID, std::string_view stageName, std::string_view after)
	{
		m_CommandBuffer.AddCommand([scheduleID, stageName, after](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.Scheduler.AddStageAfter(scheduleID, stageName, after);
			});
	}

	void Commands::AddStageBefore(ScheduleID scheduleID, std::string_view stageName, std::string_view before)
	{
		m_CommandBuffer.AddCommand([scheduleID, stageName, before](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.Scheduler.AddStageBefore(scheduleID, stageName, before);
			});
	}
}