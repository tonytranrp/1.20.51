#pragma once

#include "BlockLegacy.h"

class Block
{
public:
	char pad_0000[48]; //0x0000
	BlockLegacy* blockLegacy; //0x0030
};