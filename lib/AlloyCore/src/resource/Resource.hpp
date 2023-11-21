#pragma once

namespace Alloy
{
	/// <summary>
	/// Resources are global data used by the application.
	/// They provide a simple way to share data between systems.
	/// All resource types must inherit from this so that they can be used in systems.
	/// </summary>
	struct Resource
	{
		virtual ~Resource() = default;
	};
}