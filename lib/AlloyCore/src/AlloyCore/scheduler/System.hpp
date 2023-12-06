#pragma once
#include "AlloyCore/standard.hpp"

#include "AlloyCore/type/TypeID.hpp"
#include "StageInputs.hpp"
#include "AlloyCore/ecs/Query.hpp"
#include "AlloyCore/ecs/ComponentRegistry.hpp"
#include "AlloyCore/resource/ResourceList.hpp"

namespace Alloy
{
	template <typename T>
	concept IsResource = std::is_base_of_v<Resource, Internal::ResType::Stripped<T>>&& std::is_reference_v<T>;

	template <typename T>
	concept IsCommands = std::is_same_v<Commands, T>;

	template <typename T>
	concept IsQuery = Internal::TypeID<>::IsInstance<T, Query>::value;

	template <typename T>
	concept ValidSystemInput = IsResource<T> || IsCommands<T> || IsQuery<T>;

	/// <summary>
	/// A function pointer that represents a system.
	/// </summary>
	template <ValidSystemInput... Args>
	using System = void(*)(Args...);
}

namespace Alloy::Internal
{
	/// <summary>
	/// Holds the IDs of every type of system input.
	/// Used to check for collisions between systems, such as one system writing to a resource while another reads from it.
	/// </summary>
	class SystemInputs
	{
	public:
		std::vector<size_t> ResourceReads;
		std::vector<size_t> ResourceWrites;

		std::vector<size_t> ComponentReads;
		std::vector<size_t> ComponentWrites;

		bool HasCommands = false;

	public:
		template <typename T>
		struct SystemInputType
		{
			using Type = T&;
		};

		template <typename... Ts>
		struct SystemInputType<Query<Ts...>>
		{
			using Type = Query<Ts...>;
		};

		template <>
		struct SystemInputType<Commands>
		{
			using Type = Commands;
		};

		/// <summary>
		/// Returns the correct input (resource, component query, etc...) based on type T.
		/// This is evaluated at compile time, so there is no runtime overhead.
		/// </summary>
		template<ValidSystemInput T, typename AppState>
		static SystemInputType<T>::Type Get(AppState& appState);

		/// <summary>
		/// Adds the ID of the input to the correct list in the SystemInputs object.
		/// Return value is the ID of the input.
		/// </summary>
		template<ValidSystemInput T>
		static void Declare(SystemInputs& inputs);

		/// <summary>
		/// Checks for collisions between system inputs and updates the stage inputs accordingly.
		/// </summary>
		template <ValidSystemInput... Inputs>
		static void Check(StageInputs& stageInputs);
	};

	template<ValidSystemInput T, typename AppState>
	/*static*/ SystemInputs::SystemInputType<T>::Type SystemInputs::Get(AppState& appState)
	{
		// query
		if constexpr (IsQuery<T>)
		{
			return appState.Scene.GetQuery<T>();
		}

		// commands
		else if constexpr (IsCommands<T>)
		{
			return appState.CommandBufferPool.CreateCommands(appState);
		}

		// resources
		else if constexpr (IsResource<T>)
		{
			return appState.ResourceList.GetResource<T>();
		}

		// invalid
		else
		{
			ASSERT(false, "Not a valid input type!");
			return {};
		}
	}

	template<ValidSystemInput T>
	/*static*/ void SystemInputs::Declare(SystemInputs& inputs)
	{
		// query
		if constexpr (IsQuery<T>)
		{
			for (size_t id : T::GetReadIDs())
			{
				if (id != (size_t)-1)
					inputs.ComponentReads.push_back(id);
			}

			for (size_t id : T::GetWriteIDs())
			{
				if (id != (size_t)-1)
					inputs.ComponentWrites.push_back(id);
			}
		}

		// commands
		else if constexpr (IsCommands<T>)
		{
			inputs.HasCommands = true;
		}

		// resources
		else if constexpr (IsResource<T>)
		{
			// read
			if constexpr (std::is_const_v<T>)
			{
				inputs.ResourceReads.push_back(ResType::ID<T>());
			}

			// write
			else
			{
				inputs.ResourceWrites.push_back(ResType::ID<T>());
			}
		}

		// invalid
		else
		{
			ASSERT(false, "Not a valid input type!");
		}
	}

	template <ValidSystemInput... Inputs>
	/*static*/ void SystemInputs::Check(StageInputs& stageInputs)
	{
		SystemInputs systemInputs;

		(Declare<Inputs>(systemInputs), ...);

		// check if system uses commands
		if (systemInputs.HasCommands)
		{
			stageInputs.NumCommands += 1;
		}

		// check if new system writes to a resource already being read from or written to
		for (auto& resWriteID : systemInputs.ResourceWrites)
		{
			ASSERT(
				!stageInputs.ResourceReads.contains(resWriteID) && !stageInputs.ResourceWrites.contains(resWriteID),
				"System writes to resource already being read from or written to by another system on this stage! "
				"Consider moving the system to a different stage."
			);

			stageInputs.ResourceWrites.insert(resWriteID);
		}

		// check if new system reads from a resource already being written to
		for (auto& resReadID : systemInputs.ResourceReads)
		{
			ASSERT(
				!stageInputs.ResourceWrites.contains(resReadID),
				"System reads from resource already being written to by another system on this stage! "
				"Consider moving the system to a different stage."
			);

			stageInputs.ResourceReads.insert(resReadID);
		}

		// check if new system writes to a component already being read from or written to
		for (auto& compWriteID : systemInputs.ComponentWrites)
		{
			ASSERT(
				!stageInputs.ComponentReads.contains(compWriteID) && !stageInputs.ComponentWrites.contains(compWriteID),
				"System writes to component already being read from or written to by another system on this stage! "
				"Consider moving the system to a different stage."
			);

			stageInputs.ComponentWrites.insert(compWriteID);
		}

		// check if new system reads from a component already being written to
		for (auto& compReadID : systemInputs.ComponentReads)
		{
			ASSERT(
				!stageInputs.ComponentWrites.contains(compReadID),
				"System reads from component already being written to by another system on this stage! "
				"Consider moving the system to a different stage."
			);

			stageInputs.ComponentReads.insert(compReadID);
		}

	}

}