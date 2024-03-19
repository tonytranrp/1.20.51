#pragma once

class LookControlComponent
{
public:
	int16_t N00002ED5; //0x0000
	char pad_0002[2]; //0x0002
	int32_t N00002EE6; //0x0004
	int32_t N00002ED6; //0x0008
	int32_t N00002EEC; //0x000C
	int32_t N00002ED7; //0x0010
	int32_t N00002EEF; //0x0014
	int32_t N00002ED8; //0x0018
	int32_t N00002EF2; //0x001C
	int32_t N00002ED9; //0x0020
	int32_t N00002EF5; //0x0024
	void* N00002EDA; //0x0028
}; //Size: 0x0030
static_assert(sizeof(LookControlComponent) == 0x30);