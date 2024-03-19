#pragma once

#include <Math/Math.h>

struct MovementInterpolatorComponent
{
	Vector3<float> position; //0x0000
	Vector2<float> angles; //0x000C
	float headYaw; //0x0014
	int32_t N00002694; //0x0018
	int32_t N0000267E; //0x001C
	int32_t N00002697; //0x0020
	bool running; //0x0024
};

static_assert(sizeof(MovementInterpolatorComponent) == 0x28);