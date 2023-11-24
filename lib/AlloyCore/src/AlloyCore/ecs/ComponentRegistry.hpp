#pragma once

#include "entt/entt.hpp"

#include "AlloyCore/log/Log.hpp"
#include "EntityID.hpp"
#include "Query.hpp"

namespace Alloy::Internal
{
	/// <summary>
	/// Stores entities and their components.
	/// </summary>
	class ComponentRegistry
	{
	public:
		EntityID CreateEntityID()
		{
			return m_Registry.create();
		}

		void RemoveEntity(EntityID entityID)
		{
			m_Registry.destroy(entityID);
		}

		template <typename Query>
		Query GetQuery()
		{
			if constexpr (Query::NumComponents > 1)
			{
				return Query(m_Registry.group<>(Query::EnttGetType()));
			}

			else
			{
				return Query(m_Registry.view<typename Query::EnttGetType>());
			}
		}

		template<typename... Components>
		bool HasComponents(EntityID entityID)
		{
			return m_Registry.all_of<Components...>(entityID);
		}

		template<typename Component, typename... Args>
		Component& AddComponent(EntityID entityID, Args&&... args)
		{
			ASSERT(!HasComponents<Component>(entityID), "Entity already has component!");
			return m_Registry.emplace<Component>(entityID, std::forward<Args>(args)...);
		}

		template<typename Component>
		Component& GetComponent(EntityID entityID)
		{
			ASSERT(HasComponents<Component>(entityID), "Entity does not contain component!");
			return m_Registry.get<Component>(entityID);
		}

		template<typename Component>
		void RemoveComponent(EntityID entityID)
		{
			ASSERT(HasComponents<Component>(entityID), "Entity does not contain component!");

			m_Registry.remove<Component>(entityID);
		}

	private:
		entt::registry m_Registry;
	};
}