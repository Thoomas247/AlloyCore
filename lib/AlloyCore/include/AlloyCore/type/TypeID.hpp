#pragma once
#include "AlloyCore/standard.hpp"

namespace Alloy::Internal
{
	/// <summary>
	/// Provides a unique ID for each type T supplied to the ID() function.
	/// Different counters can be used by supplying a different Category type.
	/// ie TypeID<TypeA> is a different counter to TypeID<TypeB>.
	/// </summary>
	template <typename Category = void>
	class TypeID
	{
	public:
		template <class T>
		using Stripped = std::remove_const_t<std::remove_reference_t<std::remove_const_t<T>>>;

		/// <summary>
		/// To check if a template instantiation is an instance of a template.
		/// </summary>
		template <class, template <class, class...> class>
		struct IsInstance : public std::false_type {};

		template <class...Ts, template <class, class...> class U>
		struct IsInstance<U<Ts...>, U> : public std::true_type {};

		template <class T>
		static size_t ID()
		{
			return getID<Stripped<T>>();
		}

	private:
		inline static size_t s_Counter = 0;

		template <class T>
		static size_t getID()
		{
			static size_t id = s_Counter++;

			return id;
		}
	};

	/// <summary>
	/// A type counter used to generate unique IDs for each event type.
	/// </summary>
	using EventType = TypeID<struct EventeCategory>;

	/// <summary>
	/// A type counter used to generate unique IDs for each resource type.
	/// </summary>
	using ResType = TypeID<struct ResourceCategory>;

	/// <summary>
	/// A type counter used to generate unique IDs for each component type.
	/// </summary>
	using CompID = TypeID<struct ComponentCategory>;

}