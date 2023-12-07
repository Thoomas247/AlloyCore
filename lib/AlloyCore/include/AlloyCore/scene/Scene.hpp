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

}

namespace Alloy::Internal
{
	/// <summary>
	/// Scenes hold all the entities and components for a specific level or area of the game.
	/// </summary>
	class Scene
	{
	public:
		Scene();
		~Scene() = default;

		EntityID SpawnEntity();
		EntityID SpawnChildEntity(EntityID parentID);
		void DespawnEntity(EntityID id);

		void SetParent(EntityID childID, EntityID parentID);

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
		EntityID createEntityWithBaseComponents();

	private:
		EntityID m_RootEntityID;
		Internal::ComponentRegistry m_ComponentRegistry;
	};
}