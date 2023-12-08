#pragma once
#include "AlloyCore/standard.hpp"

#include "AlloyCore/app/AppState.hpp"
#include "AlloyCore/scheduler/Scheduler.hpp"
#include "CommandBuffer.hpp"

namespace Alloy
{
	// forward declarations
	class EntityCommands;

	/// <summary>
	/// Commands allow you to create/remove resources, entities and components.
	/// </summary>
	class Commands
	{
	public:
		Commands(Internal::CommandBuffer& commandBuffer, Internal::AppState& appState);

		/// <summary>
		/// Spawns a new entity.
		/// Returns an EntityCommands object allowing you to edit the new entity.
		/// </summary>
		EntityCommands SpawnEntity();

		/// <summary>
		/// Returns an EntityCommands object allowing you to edit the given entity.
		/// </summary>
		EntityCommands GetEntity(EntityID entityID);

		/// <summary>
		/// Destroys the given entity and all its components.
		/// </summary>
		void DestroyEntity(EntityID entityID);

		/// <summary>
		/// Adds a stage to the end of given schedule.
		/// The new stage must not already exist.
		/// </summary>
		void AddStage(ScheduleID scheduleID, std::string_view stageName);

		/// <summary>
		/// Adds a stage to the given schedule after the given stage.
		/// </summary>
		void AddStageAfter(ScheduleID scheduleID, std::string_view stageName, std::string_view after);

		/// <summary>
		/// Adds a stage to the given schedule before the given stage.
		/// </summary>
		void AddStageBefore(ScheduleID scheduleID, std::string_view stageName, std::string_view before);

		/// <summary>
		/// Adds a plugin to the application.
		/// The order in which plugins are added does not matter, as execution order is determined by the schedules.
		/// Any provided arguments are forwarded to the plugin's constructor.
		/// </summary>
		template <typename Plugin, typename... Args>
		void AddPlugin(Args&&... args);

		/// <summary>
		/// Adds a system to the given stage in the given schedule.
		/// Both the stage and the schedule must exist, otherwise an error will occur.
		/// </summary>
		template <typename... Args>
		void AddSystem(ScheduleID scheduleID, std::string_view stageName, System<Args...> system);

		/// <summary>
		/// Add a resource to the application.
		/// If the given resource already exists, it is overriden.
		/// </summary>
		template <typename T, typename... Args>
		void AddResource(Args&&... args);

		/// <summary>
		/// Add a resource to the application if it does not already exist.
		/// If the given resource already exists, it is not overriden.
		/// </summary>
		template <typename T, typename... Args>
		void AddResourceIfMissing(Args&&... args);

		/// <summary>
		/// Adds a resource to the application, which is aliased as a different type.
		/// This is useful for platform-specific resources, such as a window, 
		/// where the interface is the same but the underlying implementation may vary.
		/// For example, AddResourceAs<Dog, Animal>() would add a Dog resource, which
		/// can only be accessed as an Animal resource.
		/// If a resource with this alias already exists, it is overriden.
		/// </summary>
		template <typename T, typename As, typename... Args>
		void AddResourceAs(Args&&... args);

	protected:
		Internal::CommandBuffer& m_CommandBuffer;
		Internal::AppState& m_AppState;
	};

	template <typename Plugin, typename... Args>
	void Commands::AddPlugin(Args&&... args)
	{
		m_CommandBuffer.AddCommand([=](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.Plugins.emplace_back(std::make_unique<Plugin>(std::forward<Args>(args)...));
			});
	}

	template <typename... Args>
	void Commands::AddSystem(ScheduleID scheduleID, std::string_view stageName, System<Args...> system)
	{
		m_CommandBuffer.AddCommand([=](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.Scheduler.AddSystem(appState, scheduleID, stageName, system);
			});
	}

	template <typename T, typename... Args>
	void Commands::AddResource(Args&&... args)
	{
		m_CommandBuffer.AddCommand([=](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.ResourceList.AddResource<T>(std::forward<Args>(args)...);
			});
	}

	template <typename T, typename... Args>
	void Commands::AddResourceIfMissing(Args&&... args)
	{
		m_CommandBuffer.AddCommand([=](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.ResourceList.AddResourceIfMissing<T>(std::forward<Args>(args)...);
			});
	}

	template <typename T, typename As, typename... Args>
	void Commands::AddResourceAs(Args&&... args)
	{
		m_CommandBuffer.AddCommand([=](Internal::CommandBuffer& commandBuffer, Internal::AppState& appState)
			{
				appState.ResourceList.AddResourceAs<T, As>(std::forward<Args>(args)...);
			});
	}
}