#pragma once

#include <stdint.h>

#include "ItemDescriptor.h"

class ItemDescriptorCount : public ItemDescriptor
{
public:
	uint16_t count; //0x0010
	char pad_0012[6]; //0x0012
}; //Size: 0x0018
static_assert(sizeof(ItemDescriptorCount) == 0x18);
