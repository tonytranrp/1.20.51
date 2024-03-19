#pragma once

class UIProfanityContext
{
public:
	char pad_0000[8]; //0x0000
	bool enabled; //0x0008
	char pad_0009[7]; //0x0009
}; //Size: 0x0010
static_assert(sizeof(UIProfanityContext) == 0x10);