#pragma once

#include "Block.h"

#include <string>
#include <vector>

class BlockPalette
{
public:
	char pad_0000[104]; //0x0000
	std::vector<Block*> blocks; //0x0068

	[[nodiscard]] const Block* getBlock(const std::string& id) const
	{
		auto found = std::ranges::find_if(blocks.begin(), blocks.end(), [id](auto* block) -> bool {
			return block->blockLegacy->itemId.get() == id;
		});
		if(found != blocks.end())
		{
			return *found;
		}
		return nullptr;
	}
};