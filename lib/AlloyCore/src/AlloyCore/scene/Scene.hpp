#pragma once

#include "AlloyCore/ecs/ComponentRegistry.hpp"
#include "AlloyCore/ecs/EntityID.hpp"
#include "AlloyCore/ecs/Component.hpp"

namespace Alloy
{

	/// <summary>
	/// Component storing an identifier for the parent of this entity.
	/// Note that changing this manually will not update the children of the previous parent.
	/// Rather, you should use commands to move entities around.
	/// </summary>
	struct Parent : public Component
	{
		EntityID ParentID;

		Parent()
			: ParentID(NullEntity)
		{}
	};

	/// <summary>
	/// Component storing a list of identifiers for the children of this entity.
	/// Note that changing this manually will not update the parent of the children.
	/// Rather, you should use commands to move entities around.
	/// </summary>
	struct Children : public Component
	{
		std::vector<EntityID> ChildrenIDs;
	};

	/// <summary>
	/// Scenes hold all the entities and components for a specific level or area of the game.
	/// </summary>
	class Scene
	{
	public:
		Scene()
			: m_ComponentRegistry()
			, m_RootEntityID(NullEntity)
		{
			m_RootEntityID = createEntityWithBaseComponents();
		}

		~Scene() = default;

		EntityID SpawnEntity()
		{
			return SpawnChildEntity(m_RootEntityID);
		}

		EntityID SpawnChildEntity(EntityID parentID)
		{
			EntityID id = createEntityWithBaseComponents();

			SetParent(id, parentID);

			return id;
		}

		void DespawnEntity(EntityID id)
		{
			m_ComponentRegistry.RemoveEntity(id);
		}

		void SetParent(EntityID childID, EntityID parentID)
		{
			auto& parentComponent = m_ComponentRegistry.GetComponent<Parent>(childID);

			// get old parent
			EntityID oldParentID = parentComponent.ParentID;

			// check if not the same as new parent
			if (oldParentID == parentID)
				return;

			// remove from old parent's children list
			if (oldParentID != NullEntity)
			{
				auto& children = m_ComponentRegistry.GetComponent<Children>(oldParentID).ChildrenIDs;
				std::erase(children, childID);
			}

			// set new parent
			parentComponent.ParentID = parentID;

			// add to new parent's children list
			m_ComponentRegistry.GetComponent<Children>(parentID).ChildrenIDs.push_back(childID);
		}

		template <typename Query>
		Query GetQuery()
		{
			return m_ComponentRegistry.GetQuery<Query>();
		}

		template<typename Component, typename... Args>
		void AddComponent(EntityID entityID, Args&&... args)
		{
			m_ComponentRegistry.AddComponent<Component>(entityID, std::forward<Args>(args)...);
		}

		template<typename Component>
		void RemoveComponent(EntityID entityID)
		{
			m_ComponentRegistry.RemoveComponent<Component>(entityID);
		}

	private:
		EntityID createEntityWithBaseComponents()
		{
			EntityID id = m_ComponentRegistry.CreateEntityID();

			// add hierarchy components
			m_ComponentRegistry.AddComponent<Parent>(id);
			m_ComponentRegistry.AddComponent<Children>(id);

			return id;
		}

	private:
		EntityID m_RootEntityID;
		Internal::ComponentRegistry m_ComponentRegistry;
	};
}