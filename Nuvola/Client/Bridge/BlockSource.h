#pragma once

#include <Math/Math.h>

#include "Block.h"
#include "Macro.h"
#include "Types.h"

#include <stdint.h>
#include <vector>

struct BlockSource {
	/* Fields */
	char pad_0008[48]; //0x0008
	int16_t buildHeight; //0x0038
	int16_t buildDepth; //0x003A

	/* Virtuals */
	virtual ~BlockSource();
	const Block* getBlock(const BlockPos& blockPos) const
	{
		return vftCall<2, const Block*, const BlockSource*, const BlockPos&>(this, blockPos);
	}
	const Block* getBlock(int x, int y, int z) const
	{
		return vftCall<3, const Block*, const BlockSource*, int, int, int>(this, x, y, z);
	}
	
	/* Functions */
};