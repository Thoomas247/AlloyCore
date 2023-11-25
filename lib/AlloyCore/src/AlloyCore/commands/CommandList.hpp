#pragma once
#include "AlloyCore/standard.hpp"

#include "FutureEntityID.hpp"

namespace Alloy::Internal
{
	class CommandBuffer;

	using CommandFunction = std::function<void(CommandBuffer&, AppState&)>;

	class CommandBuffer
	{
	public:
		void Run(AppState& appState)
		{
			for (auto& command : m_Commands)
			{
				command(*this, appState);
			}
		}

		size_t CreateFutureEntityID(EntityID id = NullEntity)
		{
			m_FutureEntityIDs.emplace_back(id);

			return m_FutureEntityIDs.size() - 1;
		}

		FutureEntityID& GetFutureEntityID(size_t index)
		{
			return m_FutureEntityIDs[index];
		}

		void SetFutureEntityID(size_t index, EntityID id)
		{
			m_FutureEntityIDs[index].ID = id;
		}

		void AddCommand(CommandFunction commandFunc)
		{
			m_Commands.emplace_back(commandFunc);
		}

	private:
		std::vector<FutureEntityID> m_FutureEntityIDs;
		std::vector<CommandFunction> m_Commands;
	};

}
