#include "Schedule.hpp"

#include "AlloyCore/app/AppState.hpp"

namespace Alloy::Internal
{
	/* -- PUBLIC -- */

	void Schedule::Run(AppState& appState)
	{
		for (auto& stage : m_Stages)
		{
			stage.Run(appState);
		}
	}

	void Schedule::AddStage(std::string_view stageName)
	{
		// assert that stage doesn't already exist
		ASSERT(
			tryGetStageIt(stageName) == m_Stages.end(),
			"Stage already exists in schedule! "
			"This is an error as it means another plugin expects this schedule to be in a certain position, which may not match the position being inserted to now."
		);

		m_Stages.emplace_back(stageName);
	}

	void Schedule::AddStageAfter(std::string_view stageName, std::string_view after)
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

	void Schedule::AddStageBefore(std::string_view stageName, std::string_view before)
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


	/* -- PRIVATE -- */

	std::vector<Stage>::iterator Schedule::tryGetStageIt(std::string_view stageName)
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
}