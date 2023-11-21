#pragma once
#include "AlloyCore/standard.hpp"

#include "Stage.hpp"
#include "AlloyCore/log/Log.hpp"

namespace Alloy::Internal
{
	/// <summary>
	/// A group of stages which are executed one after the other, in order.
	/// </summary>
	class Schedule
	{
	public:
		void Run()
		{
			for (auto& stage : m_Stages)
			{
				stage.Run();
			}
		}

		void AddStage(std::string_view stageName)
		{
			// assert that stage doesn't already exist
			ASSERT(
				tryGetStageIt(stageName) == m_Stages.end(),
				"Stage already exists in schedule! "
				"This is an error as it means another plugin expects this schedule to be in a certain position, which may not match the position being inserted to now."
			);

			m_Stages.emplace_back(stageName);
		}

		void AddStageAfter(std::string_view stageName, std::string_view after)
		{
			// assert that stage doesn't already exist
			ASSERT(
				tryGetStageIt(stageName) == m_Stages.end(),
				"Stage already exists in schedule! "
				"This is an error as it means another plugin expects this schedule to be in a certain position, which may not match the position being inserted to now."
			);

			// find after stage and insert new stage after it
			auto it = tryGetStageIt(after);

			ASSERT(it != m_Stages.end(), "Could not find stage to insert after!");

			m_Stages.emplace(it + 1, stageName);
		}

		void AddStageBefore(std::string_view stageName, std::string_view before)
		{
			// assert that stage doesn't already exist
			ASSERT(
				tryGetStageIt(stageName) == m_Stages.end(),
				"Stage already exists in schedule! "
				"This is an error as it means another plugin expects this schedule to be in a certain position, which may not match the position being inserted to now."
			);

			// find before stage and insert new stage before it
			auto it = tryGetStageIt(before);

			ASSERT(it != m_Stages.end(), "Could not find stage to insert before!");

			m_Stages.emplace(it, stageName);
		}

		template <typename... Args>
		void AddSystem(AppState& appState, std::string_view stageName, System<Args...> system)
		{
			auto it = tryGetStageIt(stageName);

			ASSERT(it != m_Stages.end(), "Stage {0} does not exist in this schedule!", stageName);

			it->AddSystem(appState, system);
		}

	private:
		std::vector<Stage>::iterator tryGetStageIt(std::string_view stageName)
		{
			for (auto it = m_Stages.begin(); it != m_Stages.end(); ++it)
			{
				if (it->GetName() == stageName)
				{
					return it;
				}
			}

			return m_Stages.end();
		}

	private:
		std::vector<Stage> m_Stages;
	};
}