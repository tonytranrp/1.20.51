#pragma once

#include "Container.h"
#include "ItemStack.h"

#include <vector>

class Inventory : public Container
{
public:
	std::vector<ItemStack> stacks; //0x00E0

	int getEmptySlotFromContainer()
	{
		for (int currentSlot = 0; currentSlot < stacks.size(); currentSlot++)
		{
			const ItemStack& stack = this->stacks[currentSlot];
			if (!stack.item)
			{
				return currentSlot;
			}
		}
		return -1;
	}

	int getEmptySlotFromHotbar()
	{
		for (int inventoryRow = 3; inventoryRow > 0; inventoryRow--)
		{
			for (int currentSlot = 9 * inventoryRow; currentSlot < 9 * (inventoryRow + 1); currentSlot++)
			{
				const ItemStack& stack = this->stacks[currentSlot];
				if (!stack.item)
				{
					return currentSlot;
				}
			}
		}
		return -1;
	}
}; //Size: IDEFK