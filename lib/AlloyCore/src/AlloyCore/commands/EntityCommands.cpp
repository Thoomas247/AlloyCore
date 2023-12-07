#include "EntityCommands.hpp"

namespace Alloy
{
	/* -- PUBLIC -- */

	EntityCommands::EntityCommands(Internal::CommandBuffer& commandBuffer, Internal::AppState& appState, size_t futureEntityIDIndex)
		: Commands(commandBuffer, appState), m_FutureEntityIDIndex(futureEntityIDIndex)
	{}

	void EntityCommands::SetParent(EntityID parentID)
	{
		m_CommandBuffer.AddCommand([=, futureEntityIDIndex = m_FutureEntityIDIndex](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				// get the future entity id
				EntityID id = commandBuffer.GetFutureEntityID(futureEntityIDIndex).ID;

				// set the parent
				appState.Scene.SetParent(id, parentID);
			});
	}

	void EntityCommands::Destroy()
	{
		m_CommandBuffer.AddCommand([futureEntityIDIndex = m_FutureEntityIDIndex](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				// get the future entity id
				EntityID id = commandBuffer.GetFutureEntityID(futureEntityIDIndex).ID;

				// destroy the entity
				appState.Scene.DespawnEntity(id);
			});
	}
}