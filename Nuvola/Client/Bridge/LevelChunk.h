#pragma once

#include "SubChunk.h"

#include <stdint.h>
#include <vector>

class LevelChunk
{
public:
	char pad_0000[96]; //0x0000
	int32_t N000026BB; //0x0060
	int32_t N000027AC; //0x0064
	int32_t N000026BC; //0x0068
	int32_t N000027AF; //0x006C
	int32_t N000026BD; //0x0070
	int32_t N00002970; //0x0074
	int32_t chunkPosX; //0x0078
	int32_t chunkPosY; //0x007C
	char pad_0080[88]; //0x0080
	int8_t loadingStage; //0x00D8
	char pad_00D9[71]; //0x00D9
	int32_t mLastTick; //0x0120
	char pad_0124[20]; //0x0124
	std::vector<SubChunk> subChunks; //0x0138
	char pad_0150[3220]; //0x0150
	int32_t loadCompleted; //0x0DE4
	char pad_0DE8[979]; //0x0DE8
	bool isLoading; //0x11BB
	char pad_11BC[20]; //0x11BC

	const SubChunk& getSubChunkFor(BlockPos blockPos) const
	{
		int subChunkIndex = blockPos.y >> 4;
		return this->subChunks.at(subChunkIndex);
	}

	bool __thiscall isFullyLoaded() const
	{
		if ((8 < this->loadingStage) && (this->isLoading == '\0')) {
			return this->loadCompleted;
		}
		return true;
	}
};

static_assert(offsetof(LevelChunk, loadingStage) == 0xD8);
static_assert(offsetof(LevelChunk, loadCompleted) == 0xDE4);
static_assert(offsetof(LevelChunk, isLoading) == 0x11BB);