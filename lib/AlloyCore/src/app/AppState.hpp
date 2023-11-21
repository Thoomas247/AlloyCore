#pragma once
#include "standard.hpp"

#include "plugin/Plugin.hpp"
#include "resource/ResourceList.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "scheduler/Scheduler.hpp"

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