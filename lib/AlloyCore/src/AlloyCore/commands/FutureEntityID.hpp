#pragma once

#include "AlloyCore/ecs/EntityID.hpp"

namespace Alloy::Internal
{
	struct FutureEntityID
	{
		EntityID ID;

		FutureEntityID(EntityID id)
			: ID(id)
		{}
	};
}