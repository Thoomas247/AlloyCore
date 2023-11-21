#pragma once
#include "standard.hpp"

#include "EntityID.hpp"
#include "type/TypeID.hpp"

namespace Alloy::Internal
{
	/// <summary>
	/// Used to eveluate whether we need an entt::view or entt::group to iterate over the given components.
	/// When we have more than one component to write or read to, we use a group.
	/// </summary>
	template <size_t N, typename... Components>
	struct EnttAccessorType
	{
		using Type = entt::group<entt::owned_t<>, entt::get_t<Components...>, entt::exclude_t<>>;
		using Get = entt::get_t<Components...>;
	};

	/// <summary>
	/// Used to eveluate whether we need an entt::view or entt::group to iterate over the given components.
	/// When we have only one component to write or read to, we use a view.
	/// </summary>
	template <typename Component>
	struct EnttAccessorType<1, Component>
	{
		using Type = entt::view<entt::get_t<Component>>;
		using Get = Component;
	};

}

namespace Alloy
{
	/// <summary>
	/// Queries allow systems to access entities and their components.
	/// Components can be accessed as either read-only or read-write, by specifying the component type as const or non-const.
	/// </summary>
	template <typename... Components>
	class Query
	{
	public:
		class Iterator;

	public:
		template<typename... Components>
		std::tuple<Components&...> Get(EntityID entityID)
		{
			return m_Accessor.get<Components...>(entityID);
		}

		Iterator begin()
		{
			return Iterator(m_Accessor.begin());
		}

		Iterator end()
		{
			return Iterator(m_Accessor.end());
		}

	private:
		friend class ComponentRegistry;
		friend class Application;

		using EnttGetType = Internal::EnttAccessorType<sizeof...(Components), Components...>::Get;	// used by ComponentRegistry
		using EnttAccessor = Internal::EnttAccessorType<sizeof...(Components), Components...>::Type;	// used by ComponentRegistry

	private:
		inline static constexpr size_t NumComponents = sizeof...(Components);

		EnttAccessor m_Accessor;

	private:
		static constexpr void IsQuery() {}	// used by Application

		static constexpr std::vector<size_t> GetReadIDs()	// used by Application
		{
			return { getIDIfRead<Components>()... };
		}

		static constexpr std::vector<size_t> GetWriteIDs()	// used by Application
		{
			return { getIDIfWrite<Components>()... };
		}

		template <typename T>
		static constexpr size_t getIDIfRead()
		{
			if constexpr (std::is_const_v<T>)
				return CompID::ID<T>();
			else
				return (size_t)-1;
		}

		template <typename T>
		static constexpr size_t getIDIfWrite()
		{
			if constexpr (!std::is_const_v<T>)
				return CompID::ID<T>();
			else
				return (size_t)-1;
		}



		Query(EnttAccessor accessor)
			: m_Accessor(accessor)
		{
		}
	};

	/// <summary>
	/// Iterator for a ComponentQuery.
	/// </summary>
	template <typename... Components>
	class Query<Components...>::Iterator
	{
	private:
		EnttAccessor::iterator m_Iterator;

	public:
		Iterator(EnttAccessor::iterator iterator)
			: m_Iterator(iterator)
		{
		}

		Iterator& operator++()
		{
			++m_Iterator;
			return *this;
		}

		bool operator!=(const Iterator& other) const
		{
			return m_Iterator != other.m_Iterator;
		}

		EntityID operator*() const
		{
			return *m_Iterator;
		}
	};

}