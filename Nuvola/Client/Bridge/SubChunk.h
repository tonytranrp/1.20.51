#pragma once

#include "Block.h"
#include "SubChunkBlockStorage.h"
#include "SubChunkStorage.h"

#include <memory>

class SubChunk
{
public:
	char pad_0000[32]; //0x0000
	bool needsInitLighting; //0x0020
	bool needsClientLighting; //0x0021
	char pad_0022[6]; //0x0022
	std::unique_ptr<SubChunkStorage<Block>>* blocks; //0x0028
	char pad_0030[8]; //0x0030
	SubChunkBlockStorage* blockReadPtr; //0x0038
	char pad_0040[24]; //0x0040
	int8_t N000027E5; //0x0058
	int8_t subchunkIndex; //0x0059
	char pad_005A[6]; //0x005A

	[[nodiscard]] const Block* getBlockFor(BlockPos blockPos) const
	{
		auto readPtr = this->blockReadPtr;
		uint16_t elementId = (blockPos.x * 0x10 + blockPos.z) * 0x10 + (blockPos.y & 0xf);
		return readPtr->getElement(elementId);
	}
	[[nodiscard]] bool hasBlock(const Block* block) const
	{
		gsl::span<const Block*> palette;
		palette = this->blockReadPtr->getPaletteSnapshot(&palette);
		return std::ranges::any_of(palette.begin(), palette.end(), [&](auto val) {
			return val == block;
		});
	}[[nodiscard]] bool hasBlock(BlockId block) const
	{
		gsl::span<const Block*> palette;
		palette = this->blockReadPtr->getPaletteSnapshot(&palette);
		return std::ranges::any_of(palette.begin(), palette.end(), [&](auto val) {
			return val->blockLegacy->blockId == block;
		});
	}

}; //Size: 0x0060
static_assert(sizeof(SubChunk) == 0x60);