#pragma once
#include "AlloyCore/standard.hpp"

namespace Alloy::Internal
{
	struct StageInputs
	{
		std::unordered_set<size_t> ResourceReads;
		std::unordered_set<size_t> ResourceWrites;

		std::unordered_set<size_t> ComponentReads;
		std::unordered_set<size_t> ComponentWrites;

		std::unordered_set<size_t> EventReads;
		std::unordered_set<size_t> EventWrites;
	};
}