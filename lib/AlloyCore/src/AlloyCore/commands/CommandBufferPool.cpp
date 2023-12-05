#include "CommandBufferPool.hpp"

#include "Commands.hpp"

namespace Alloy::Internal
{
	/* -- PUBLIC -- */

	CommandBufferPool::CommandBufferPool()
		: m_CommandBuffers(), m_CurrentBufferIndex(0)
	{}

	void CommandBufferPool::Reset(size_t numBuffers)
	{
		m_CommandBuffers.clear();
		m_CommandBuffers.resize(numBuffers);
	}

	void CommandBufferPool::RunAll(AppState& appState)
	{
		for (auto& commandBuffer : m_CommandBuffers)
		{
			commandBuffer.Run(appState);
		}
	}

	Commands CommandBufferPool::CreateCommands(AppState& appState)
	{
		ASSERT(m_CurrentBufferIndex < m_CommandBuffers.size(), "No more free command buffers! Make sure the correct amount was allocated using Reset().");

		return Commands(m_CommandBuffers[m_CurrentBufferIndex++], appState);
	}
}