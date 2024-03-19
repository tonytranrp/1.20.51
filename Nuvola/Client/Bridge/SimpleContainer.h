#pragma once

#include "Container.h"
#include "ItemStack.h"

#include <vector>
#include <stdint.h>

class SimpleContainer : public Container
{
public:
	int32_t containerSize; //0x00E0
	char pad_00E4[4]; //0x00E4
	std::vector<ItemStack> stacks; //0x00E8
}; //Size: 0x0100
