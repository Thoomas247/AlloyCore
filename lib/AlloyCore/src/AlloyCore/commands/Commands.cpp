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
		// create a future entity id, which will be set when the command is executed
		size_t futureEntityIDIndex = m_CommandBuffer.CreateFutureEntityID();

		// add the command to the list
		// we capture the future entity id index so we can set it when the command is executed
		m_CommandBuffer.AddCommand([futureEntityIDIndex](Internal::CommandBuffer& commandList, Internal::AppState& appState)
			{
				// spawn an entity
				EntityID id = appState.Scene.SpawnEntity();

				// set the future entity id in case a future command needs it
				commandList.SetFutureEntityID(futureEntityIDIndex, id);
			});

		return EntityCommands(m_CommandBuffer, m_AppState, futureEntityIDIndex);
	}
}