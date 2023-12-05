#pragma once
#include "AlloyCore/standard.hpp"

#include "AlloyCore/plugin/Plugin.hpp"
#include "AlloyCore/resource/ResourceList.hpp"
#include "AlloyCore/scheduler/Scheduler.hpp"
#include "AlloyCore/commands/CommandBufferPool.hpp"
#include "AlloyCore/scene/Scene.hpp"

namespace Alloy::Internal
{
	/// <summary>
	/// Holds the state of the application.
	/// </summary>
	struct AppState
	{
		std::vector<std::unique_ptr<Plugin>> Plugins;
		Scheduler Scheduler;
		ResourceList ResourceList;
		CommandBufferPool CommandBufferPool;

		Scene Scene;
	};
}