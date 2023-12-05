#pragma once
#include "AlloyCore/standard.hpp"

#include "AlloyCore/app/AppState.hpp"
#include "CommandBuffer.hpp"

namespace Alloy
{
	// forward declarations
	class EntityCommands;

	/// <summary>
	/// Commands allow you to create/remove resources, entities and components.
	/// </summary>
	class Commands
	{
	public:
		Commands(Internal::CommandBuffer& commandBuffer, Internal::AppState& appState);

		EntityCommands SpawnEntity();

	protected:
		Internal::CommandBuffer& m_CommandBuffer;
		Internal::AppState& m_AppState;
	};
}