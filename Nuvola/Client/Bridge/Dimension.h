#pragma once

#include "BlockSource.h"

struct Dimension {
	/* Fields */
	char pad_0000[208]; //0x0000
	class BlockSource* blockSource; //0x00D0
	char pad_00D8[104]; //0x00D8
	std::string name; //0x0140
	uint32_t id; //0x0160
	char pad_0164[52]; //0x0164
	class ChunkSource* chunkSource; //0x0198
	/* Virtuals */
	/* Functions */
	class BlockSource* getBlockSource() const
	{
		return this->blockSource;
	}

	class ChunkSource* getChunkSource() const
	{
		return this->chunkSource;
	}

	const std::string& getDimensionName() const
	{
		return this->name;
	}
	uint32_t getDimensionId() const
	{
		return this->id;
	}
};