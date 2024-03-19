#pragma once

#include "../Bridge/Types.h"

#include "BlockLegacyEvent.h"
#include "Event.h"

class BlockStartDestroyEvent
{

	BlockPos* blockpos;
public:
	BlockStartDestroyEvent(BlockPos* blockpos) {
		this->blockpos = blockpos;
	}

	BlockPos* getBlockPos() {
		return this->blockpos;
	}
};