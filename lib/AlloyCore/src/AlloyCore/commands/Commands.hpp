#pragma once
#include "AlloyCore/standard.hpp"

#include "AlloyCore/app/AppState.hpp"
#include "CommandList.hpp"

namespace Alloy
{

	/// <summary>
	/// Commands allow you to create/remove resources, entities and components.
	/// </summary>
	class Commands
	{
	public:
		Commands(Internal::CommandList& commandList, Internal::AppState& appState)
			: m_CommandList(commandList), m_AppState(appState)
		{
		}

		EntityCommands SpawnEntity();

	protected:
		Internal::CommandList& m_CommandList;
		Internal::AppState& m_AppState;
	};

	class EntityCommands : public Commands
	{
	public:
		EntityCommands(Internal::CommandList& commandList, Internal::AppState& appState, size_t futureEntityIDIndex)
			: Commands(commandList, appState), m_FutureEntityIDIndex(futureEntityIDIndex)
		{}

		template<typename Component, typename... Args>
		void AddComponent(Args... args)
		{
			m_CommandList.AddCommand([=](Internal::CommandList& commandList, Internal::AppState& appState)
				{
					// get the future entity id
					EntityID id = commandList.GetFutureEntityID(m_FutureEntityIDIndex).ID;

					// add the component
					appState.Scene.AddComponent<Component>(id, args...);
				});
		}

		template<typename Component>
		void RemoveComponent()
		{
			m_CommandList.AddCommand([=](Internal::CommandList& commandList, Internal::AppState& appState)
				{
					// get the future entity id
					EntityID id = commandList.GetFutureEntityID(m_FutureEntityIDIndex).ID;

					// remove the component
					appState.Scene.RemoveComponent<Component>(id);
				});
		}

		void SetParent(EntityID parentID)
		{
			m_CommandList.AddCommand([=](Internal::CommandList& commandList, Internal::AppState& appState)
				{
					// get the future entity id
					EntityID id = commandList.GetFutureEntityID(m_FutureEntityIDIndex).ID;

					// set the parent
					appState.Scene.SetParent(id, parentID);
				});
		}

	private:
		size_t m_FutureEntityIDIndex;
	};


	EntityCommands Commands::SpawnEntity()
	{
		// create a future entity id, which will be set when the command is executed
		size_t futureEntityIDIndex = m_CommandList.CreateFutureEntityID();

		// add the command to the list
		// we capture the future entity id index so we can set it when the command is executed
		m_CommandList.AddCommand([futureEntityIDIndex](Internal::CommandList& commandList, Internal::AppState& appState)
			{
				// spawn an entity
				EntityID id = appState.Scene.SpawnEntity();

				// set the future entity id in case a future command needs it
				commandList.SetFutureEntityID(futureEntityIDIndex, id);
			});

		return EntityCommands(m_CommandList, m_AppState, futureEntityIDIndex);
	}
}