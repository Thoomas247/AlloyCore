#include "AlloyCore/commands/CommandBuffer.hpp"

namespace Alloy::Internal
{
	/* -- PUBLIC -- */

	void CommandBuffer::Run(AppState& appState)
	{
		for (auto& command : m_Commands)
		{
			command(*this, appState);
		}
	}

	size_t CommandBuffer::CreateFutureEntityID(EntityID id)
	{
		m_FutureEntityIDs.emplace_back(id);

		return m_FutureEntityIDs.size() - 1;
	}

	FutureEntityID& CommandBuffer::GetFutureEntityID(size_t index)
	{
		return m_FutureEntityIDs[index];
	}

	void CommandBuffer::SetFutureEntityID(size_t index, EntityID id)
	{
		m_FutureEntityIDs[index].ID = id;
	}

	void CommandBuffer::AddCommand(CommandFunction commandFunc)
	{
		m_Commands.emplace_back(commandFunc);
	}
}