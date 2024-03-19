#pragma once

#include <Math/Math.h>

struct AABBShapeComponent
{
public:
	AABB<float> aabb;
	float width; //0x0018
	float height; //0x001C
}; //Size: 0x0020