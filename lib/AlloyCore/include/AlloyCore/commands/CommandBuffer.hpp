#pragma once
#include "AlloyCore/standard.hpp"

#include "FutureEntityID.hpp"

namespace Alloy::Internal
{
	// forward declarations
	class CommandBuffer;
	struct AppState;

	using CommandFunction = std::function<void(CommandBuffer&, AppState&)>;

	class CommandBuffer
	{
	public:
		CommandBuffer() = default;
		~CommandBuffer() = default;

		void Run(AppState& appState);

		size_t CreateFutureEntityID(EntityID id = NullEntity);
		FutureEntityID& GetFutureEntityID(size_t index);
		void SetFutureEntityID(size_t index, EntityID id);

		void AddCommand(CommandFunction commandFunc);

	private:
		std::vector<FutureEntityID> m_FutureEntityIDs;
		std::vector<CommandFunction> m_Commands;
	};

}
