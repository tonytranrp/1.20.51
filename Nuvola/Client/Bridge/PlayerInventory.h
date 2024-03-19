#pragma once

#include <stdint.h>

#include "Inventory.h"

class PlayerInventory
{
public:
	char pad_0000[16]; //0x0000
	int64_t selectedSlot; //0x0010
	char pad_0018[168]; //0x0018
	Inventory* container; //0x00C0

	const ItemStack& getSelectedStack() const
	{
		return this->container->stacks[this->selectedSlot];
	}
}; //Size: 0x00C8
static_assert(sizeof(PlayerInventory) == 0xC8);