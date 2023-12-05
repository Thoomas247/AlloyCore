#include "EntityCommands.hpp"

namespace Alloy
{
	/* -- PUBLIC -- */

	EntityCommands::EntityCommands(Internal::CommandBuffer& commandBuffer, Internal::AppState& appState, size_t futureEntityIDIndex)
		: Commands(commandBuffer, appState), m_FutureEntityIDIndex(futureEntityIDIndex)
	{}

	void EntityCommands::SetParent(EntityID parentID)
	{
		m_CommandBuffer.AddCommand([=](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				// get the future entity id
				EntityID id = commandBuffer.GetFutureEntityID(m_FutureEntityIDIndex).ID;

				// set the parent
				appState.Scene.SetParent(id, parentID);
			});
	}
}