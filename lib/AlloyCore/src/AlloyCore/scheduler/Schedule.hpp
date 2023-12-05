#pragma once
#include "AlloyCore/standard.hpp"

#include "AlloyCore/commands/CommandBufferPool.hpp"
#include "AlloyCore/log/Log.hpp"
#include "Stage.hpp"

namespace Alloy::Internal
{
	// forward declarations
	struct AppState;

	/// <summary>
	/// A group of stages which are executed one after the other, in order.
	/// </summary>
	class Schedule
	{
	public:
		void Run(AppState& appState);

		void AddStage(std::string_view stageName);

		void AddStageAfter(std::string_view stageName, std::string_view after);

		void AddStageBefore(std::string_view stageName, std::string_view before);

		template <typename... Args>
		void AddSystem(AppState& appState, std::string_view stageName, System<Args...> system)
		{
			auto it = tryGetStageIt(stageName);

			ASSERT(it != m_Stages.end(), "Stage {0} does not exist in this schedule!", stageName);

			it->AddSystem(appState, system);
		}

	private:
		std::vector<Stage>::iterator tryGetStageIt(std::string_view stageName);

	private:
		std::vector<Stage> m_Stages;
	};
}