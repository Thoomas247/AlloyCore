#pragma once
#include "Commands.hpp"

namespace Alloy
{
	class EntityCommands : public Commands
	{
	public:
		EntityCommands(Internal::CommandBuffer& commandBuffer, Internal::AppState& appState, size_t futureEntityIDIndex);

		void SetParent(EntityID parentID);

		template<typename Component, typename... Args>
		void AddComponent(Args... args);

		template<typename Component>
		void RemoveComponent();

	private:
		size_t m_FutureEntityIDIndex;
	};

	template<typename Component, typename... Args>
	void EntityCommands::AddComponent(Args... args)
	{
		m_CommandBuffer.AddCommand([=](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				// get the future entity id
				EntityID id = commandBuffer.GetFutureEntityID(m_FutureEntityIDIndex).ID;

				// add the component
				appState.Scene.AddComponent<Component>(id, args...);
			});
	}

	template<typename Component>
	void EntityCommands::RemoveComponent()
	{
		m_CommandBuffer.AddCommand([=](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				// get the future entity id
				EntityID id = commandBuffer.GetFutureEntityID(m_FutureEntityIDIndex).ID;

				// remove the component
				appState.Scene.RemoveComponent<Component>(id);
			});
	}
}