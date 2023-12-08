#pragma once
#include "AlloyCore/standard.hpp"

#include "AppState.hpp"

// includes for convenience
#include "AlloyCore/resource/Resource.hpp"
#include "AlloyCore/ecs/Component.hpp"
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
		/// Call after adding all plugins.
		/// </summary>
		void Run();

		template <typename Plugin, typename... Args>
		void AddPlugin(Args&&... args)
		{
			m_State.Plugins.emplace_back(std::make_unique<Plugin>(std::forward<Args>(args)...));
		}

	private:
		Internal::AppState m_State;
	};

}