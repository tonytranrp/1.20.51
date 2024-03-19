#pragma once

#include <Math/Math.h>

struct RotationInput
{
	Vector2<float> angles; //0x0000
	float delta; //0x0008
	Vector2<float> yawRange; //0x000C
}; //Size: 0x0014
static_assert(sizeof(RotationInput) == 0x14);