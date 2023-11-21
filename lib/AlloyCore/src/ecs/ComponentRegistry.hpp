#pragma once

#include "entt/entt.hpp"

#include "log/Log.hpp"
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

		template<typename... _Components>
		bool HasComponents(EntityID entityID)
		{
			return m_Registry.all_of<_Components...>(entityID);
		}

		template<typename _Component, typename... _Args>
		_Component& AddComponent(EntityID entityID, _Args&&... args)
		{
			AL_ASSERT(!HasComponents<_Component>(entityID), "Entity already has component!");
			return m_Registry.emplace<_Component>(entityID, std::forward<_Args>(args)...);
		}

		template<typename _Component>
		_Component& GetComponent(EntityID entityID)
		{
			AL_ASSERT(HasComponents<_Component>(entityID), "Entity does not contain component!");
			return m_Registry.get<_Component>(entityID);
		}

		template<typename _Component>
		void RemoveComponent(EntityID entityID)
		{
			AL_ASSERT(HasComponents<_Component>(entityID), "Entity does not contain component!");

			m_Registry.remove<_Component>(entityID);
		}

	private:
		entt::registry m_Registry;
	};
}