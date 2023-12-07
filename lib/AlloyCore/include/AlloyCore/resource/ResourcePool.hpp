#pragma once
#include "AlloyCore/standard.hpp"

#include "AlloyCore/log/Log.hpp"
#include "AlloyCore/type/TypeID.hpp"
#include "Resource.hpp"

namespace Alloy::Internal
{

	/// <summary>
	/// Stores all resources used by an Application.
	/// Each resource must have a unique type.
	/// </summary>
	class ResourcePool
	{
	public:
		ResourcePool() = default;
		~ResourcePool() = default;

		/// <summary>
		/// Add a resource to the list.
		/// If the given resource exists, it is overriden.
		/// </summary>
		template <typename T, typename... Args>
		void AddResource(Args&&... args)
		{
			static_assert(std::is_base_of_v<Resource, T>, "T must be a Resource!");

			m_Resources.emplace(ResType::ID<T>(), std::make_unique<T>(std::forward<Args>(args)...));
		}

		/// <summary>
		/// Add a resource to the list if it does not already exist.
		/// If the given resource exists, nothing happens.
		/// </summary>
		template <typename T, typename... Args>
		void AddResourceIfMissing(Args&&... args)
		{
			static_assert(std::is_base_of_v<Resource, T>, "T must be a Resource!");

			// if resource already exists, do nothing
			if (m_Resources.contains(ResType::ID<T>()))
				return;

			m_Resources.emplace(ResType::ID<T>(), std::make_unique<T>(std::forward<Args>(args)...));
		}

		/// <summary>
		/// Add a resource to the list.
		/// 'As' must be a base class of 'T'.
		/// If the given 'As' resource already exists, it is overriden.
		/// </summary>
		template <typename T, typename As, typename... Args>
		void AddResourceAs(Args&&... args)
		{
			static_assert(std::is_base_of_v<As, T>, "T must be a subtype of As!");
			static_assert(std::is_base_of_v<Resource, As>, "As must be a Resource!");

			// add the resource T at location As
			m_Resources.emplace(ResType::ID<As>(), std::make_unique<T>(std::forward<Args>(args)...));
		}

		template <typename T>
		T& GetResource()
		{
			static_assert(std::is_base_of_v<Resource, ResType::Stripped<T>>, "T must be a Resource!");

			ASSERT(m_Resources.contains(ResType::ID<T>()), "Resource does not exist!");

			return *static_cast<std::remove_reference_t<T>*>(m_Resources.at(ResType::ID<T>()).get());
		}

	private:
		std::unordered_map<size_t, std::unique_ptr<Resource>> m_Resources;
	};

}