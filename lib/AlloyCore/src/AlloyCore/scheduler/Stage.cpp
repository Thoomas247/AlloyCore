#include "AlloyCore/scheduler/Stage.hpp"

#include "AlloyCore/app/AppState.hpp"

namespace Alloy::Internal
{

	/* -- PUBLIC -- */

	Stage::Stage(std::string_view name)
		: m_Name(name)
	{
	}

	const std::string_view& Stage::GetName() const { return m_Name; }

	void Stage::Run(AppState& appState)
	{
		// allocate the required number of command buffers
		if (m_StageInputs.NumCommands > 0)
			appState.CommandBufferPool.Reset(m_StageInputs.NumCommands);

		std::for_each(std::execution::par, m_Systems.begin(), m_Systems.end(), [](auto& system)
			{
				system();
			});

		if (m_StageInputs.NumCommands > 0)
			appState.CommandBufferPool.RunAll(appState);
	}
}