#pragma once
#include "AlloyCore/standard.hpp"
#include "AlloyCore/log/Log.hpp"

#include "CommandBuffer.hpp"

namespace Alloy
{
	// forward declarations
	class Commands;
}

namespace Alloy::Internal
{
	// forward declarations
	struct AppState;

	class CommandBufferPool
	{
	public:
		CommandBufferPool();
		~CommandBufferPool() = default;

		/// <summary>
		/// Resets the command buffer pool and reallocates the given number of command buffers.
		/// Called by the main thread only.
		/// </summary>
		void Reset(size_t numBuffers);

		/// <summary>
		/// Runs all command buffers in the pool.
		/// Called by the main thread only.
		/// </summary>
		void RunAll(AppState& appState);

		/// <summary>
		/// Returns an unused commands object.
		/// Called by the main thread only.
		/// </summary> 
		Commands CreateCommands(AppState& appState);

	private:
		size_t m_CurrentBufferIndex;
		std::vector<CommandBuffer> m_CommandBuffers;
	};
}