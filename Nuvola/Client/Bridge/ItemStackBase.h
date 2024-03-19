#pragma once

#include "Block.h"
#include "CompoundTag.h"
#include "Item.h"
#include "WeakPtr.h"

#include <optional>
#include <string>
#include <stdint.h>

#pragma pack(push, 1)
class ItemStackBase
{
public:
	WeakPtr<Item> item; //0x0008
	CompoundTag* nbt; //0x0010
	Block* block; //0x0018
	int16_t uses; //0x0020
	int8_t stackCount; //0x0022
	char pad_0023[101]; //0x0023

	virtual ~ItemStackBase() {};
	virtual void reinit(std::string param_1, int param_2, int param_3) {};
	virtual void reinit(class BlockLegacy* param_1, int param_2) {};
	virtual void reinit(class Item* param_1, int param_2, int param_3) {};
	virtual void setNull(std::optional<std::string> param_1 = {}) {};
	virtual std::string toString() { return "INVALID"; };
	virtual std::string toDebugString() { return "INVALID"; };

	bool isBlockItem()
	{
		if (item)
		{
			Item* itemPtr = this->item.get();
			if (itemPtr->blockLegacy)
				return true;
			return false;
		}
		return false;
	}
};
#pragma pack(pop)

static_assert(sizeof(ItemStackBase) == 0x88);