#pragma once
#include "AlloyCore/standard.hpp"

//#include "AlloyCore/commands/Commands.hpp"

namespace Alloy
{
	class Commands;

	class Plugin
	{
	public:
		virtual void Build(Commands commands) = 0;
	};
}