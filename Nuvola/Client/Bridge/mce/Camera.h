#pragma once

#include <Math/Math.h>

namespace mce
{
	class Camera
	{
	public:
		char pad_0000[192]; //0x0000
		Matrix16<float> worldMatrix; //0x00C0
		char pad_0100[320]; //0x0100
	}; //Size: 0x0240
	static_assert(sizeof(Camera) == 0x240);
}