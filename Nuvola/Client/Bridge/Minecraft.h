#pragma once

#include "MCTimer.h"

class Minecraft
{
public:
	char pad_0000[216]; //0x0000
	MCTimer* tickTimer; //0x00D8
	MCTimer* renderTimer; //0x00E0
}; //Size: 0x00E0