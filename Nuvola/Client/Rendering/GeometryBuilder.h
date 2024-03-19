#pragma once

#include "IRenderer.h"

#include <Math/Math.h>

class GeometryBuilder
{
	IRenderer* renderer;
public:
	GeometryBuilder(IRenderer* renderer);
	~GeometryBuilder();

	void buildAABB(AABB<float> box, ColorF color, bool filled, float thickness = 2.0f);
	void buildBlock(const Vector3<float>& blockPos, ColorF color, bool filled, float thickness = 2.0f);
};