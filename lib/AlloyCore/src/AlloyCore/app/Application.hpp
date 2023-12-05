#pragma once
#include "AlloyCore/standard.hpp"

#include "AppState.hpp"
#include "AlloyCore/scheduler/System.hpp"
#include "AlloyCore/resource/Resource.hpp"
#include "AlloyCore/ecs/Query.hpp"
#include "AlloyCore/commands/Commands.hpp"
#include "AlloyCore/commands/EntityCommands.hpp"

namespace Alloy
{
	/// <summary>
	/// Resource containing the app's current status.
	/// Systems can write to this resource to stop the application.
	/// </summary>
	struct AppStatus : public Resource
	{
		bool Running = true;
	};

	/// <summary>
	/// Represents the user's application.
	/// Each application is made up of plugins which add functionality to the application.
	/// Each plugin may add its own systems to the application, and each system is checked against others
	/// in the same stage to ensure that there are no conflicts when running these systems in parallel.
	/// </summary>
	class Application
	{
	public:
		/// <summary>
		/// Application constructor.
		/// Creates the default stages and resources.
		/// </summary>
		Application();
		~Application() = default;

		/// <summary>
		/// Runs the application.
		/// Call after adding all plugins, stages, and systems.
		/// </summary>
		void Run();

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
		void AddPlugin(Args&&... args)
		{
			m_State.Plugins.emplace_back(std::make_unique<Plugin>(std::forward<Args>(args)...));
		}

		/// <summary>
		/// Adds a system to the given stage in the given schedule.
		/// Both the stage and the schedule must exist, otherwise an error will occur.
		/// </summary>
		template <typename... Args>
		void AddSystem(ScheduleID scheduleID, std::string_view stageName, System<Args...> system)
		{
			m_State.Scheduler.AddSystem(m_State, scheduleID, stageName, system);
		}

		/// <summary>
		/// Add a resource to the application.
		/// If the given resource already exists, it is overriden.
		/// </summary>
		template <typename T, typename... Args>
		void AddResource(Args&&... args)
		{
			m_State.ResourceList.AddResource<T>(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Add a resource to the application if it does not already exist.
		/// If the given resource already exists, it is not overriden.
		/// </summary>
		template <typename T, typename... Args>
		void AddResourceIfMissing(Args&&... args)
		{
			m_State.ResourceList.AddResourceIfMissing<T>(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Adds a resource to the application, which is aliased as a different type.
		/// This is useful for platform-specific resources, such as a window, 
		/// where the interface is the same but the underlying implementation may vary.
		/// For example, AddResourceAs<Dog, Animal>() would add a Dog resource, which
		/// can only be accessed as an Animal resource.
		/// If a resource with this alias already exists, it is overriden.
		/// </summary>
		template <typename T, typename As, typename... Args>
		void AddResourceAs(Args&&... args)
		{
			m_State.ResourceList.AddResourceAs<T, As>(std::forward<Args>(args)...);
		}

	private:
		Internal::AppState m_State;
	};

}