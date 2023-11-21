#pragma once
#include "AlloyCore/standard.hpp"

#include "AlloyCore/plugin/Plugin.hpp"
#include "AlloyCore/resource/ResourceList.hpp"
#include "AlloyCore/ecs/ComponentRegistry.hpp"
#include "AlloyCore/scheduler/Scheduler.hpp"

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

		ComponentRegistry ComponentRegistry;	// TODO: scenes should own this
	};
}