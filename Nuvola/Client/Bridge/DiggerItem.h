#pragma once
#include "Item.h"
#include "ItemStackBase.h"

class DiggerItem : Item {
public:
	float getDestroySpeed(const ItemStackBase* itemstackbase, const Block* block) const
	{
		return vftCall<88, float, const Item*, const ItemStackBase*, const Block*>(this, itemstackbase, block);
	}
};