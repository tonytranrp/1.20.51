#pragma once

class FloatOption
{
public:
	char pad_0000[16]; //0x0000
	float minimum_2; //0x0010
	float maximum; //0x0014
	float value; //0x0018
	float minimum; //0x001C
}; //Size: 0x0020