#pragma once
#include "AlloyCore/standard.hpp"

namespace Alloy
{
	class Application;

	class Plugin
	{
	public:
		virtual void Build(Application& app) = 0;
	};
}