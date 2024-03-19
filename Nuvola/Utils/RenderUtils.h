#pragma once

#include <Math/Math.h>

#include <imgui.h>

namespace RenderUtils {
	bool DrawPos(const Vector3<float>& pos, ImU32 color, float thickness = 2.0f);
	bool DrawAABB(const AABB<>& aabb, ImU32 espCol, float thickness = 2.0f);
	bool DrawBlock(const Vector3<int>& blockPos, ImU32 color, float thickness = 2.0f);
}