#pragma once
#include "AlloyCore/standard.hpp"

#include "System.hpp"
#include "StageInputs.hpp"

namespace Alloy
{
	constexpr auto DEFAULT_STARTUP_STAGE = "Startup";
	constexpr auto DEFAULT_UPDATE_STAGE = "Update";
	constexpr auto DEFAULT_RENDER_STAGE = "Render";
	constexpr auto DEFAULT_SHUTDOWN_STAGE = "Shutdown";
}

namespace Alloy::Internal
{
	// forward declarations
	struct AppState;

	/// <summary>
	/// A group of systems which can be executed in parallel.
	/// </summary>
	class Stage
	{
	public:
		Stage(std::string_view name)
			: m_Name(name)
		{
		}

		const std::string_view& GetName() const { return m_Name; }

		void Run()
		{
			std::for_each(std::execution::par, m_Systems.begin(), m_Systems.end(), [](auto& system) {
				system();
				});
		}

		template <typename... Args>
		void AddSystem(AppState& appState, System<Args...> system)
		{
			// check the inputs for collisions
			SystemInputs::Check<Args...>(m_StageInputs);

			// add system to stage
			m_Systems.emplace_back([system, &appState]() {
				system(SystemInputs::Get<Args>(appState)...);
				});
		}

	private:
		std::string_view m_Name;

		StageInputs m_StageInputs;
		std::vector<std::function<void()>> m_Systems;
	};
}