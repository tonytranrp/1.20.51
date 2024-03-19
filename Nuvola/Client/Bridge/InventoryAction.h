#pragma once

#include <stdint.h>

#include "InventorySource.h"
#include "ItemStack.h"
#include "NetworkItemStackDescriptor.h"

class InventoryAction
{
public:
	InventorySource source;
	uint32_t slot; //0x000C
	NetworkItemStackDescriptor fromDescriptor; //0x0010
	NetworkItemStackDescriptor toDescriptor; //0x0070
	ItemStack fromItem; //0x00D0
	ItemStack toItem; //0x0170

	InventoryAction(InventorySource source, uint32_t slot, const ItemStack& from, const ItemStack& to)
	{
		this->source = source;
		this->slot = slot;
		NetworkItemStackDescriptor fromDescriptor(from);
		NetworkItemStackDescriptor toDescriptor(to);
		memcpy(&this->fromDescriptor, &fromDescriptor, sizeof(ItemStack));
		memcpy(&this->toDescriptor, &toDescriptor, sizeof(ItemStack));
		memcpy(&this->fromItem, &from, sizeof(ItemStack));
		memcpy(&this->toItem, &to, sizeof(ItemStack));
	}
};
static_assert(sizeof(InventoryAction) == 0x210);