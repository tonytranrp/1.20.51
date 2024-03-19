#include "RenderUtils.h"

#include "../Client/Bridge/ClientInstance.h"
#include "../Client/Bridge/GuiData.h"
#include "../Client/Bridge/LocalPlayer.h"
#include "../Client/Bridge/LevelRenderer.h"

bool RenderUtils::DrawPos(const Vector3<float>& pos, ImU32 color, float thickness)
{
	Vector3<float> cornerA = pos - 0.5f;
	cornerA.y = pos.y;
	Vector3<float> cornerB = pos + 0.5f;
	cornerB.y = pos.y;
	AABB<float> square(
		cornerA,
		cornerB
	);
	return RenderUtils::DrawAABB(
		square,
		color,
		thickness
	);
}

bool RenderUtils::DrawAABB(const AABB<>& aabb, ImU32 espCol, float thickness) {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return false;
	GuiData* gd = ci->getGuiData();
	if (!gd) return false;
	LocalPlayer* lp = ci->getClientPlayer();
	if (!lp) return false;
	if (!ci->mLevelRenderer) return false;

	LevelRendererPlayer* lr = ci->mLevelRenderer->levelRendererPlayer;
	if (!lr) return false;

	Vector3<float> cornerA = aabb.firstCorner;
	Vector3<float> cornerH = aabb.secondCorner;
	Vector3<float> cornerB = { cornerH.x, cornerA.y, cornerA.z };
	Vector3<float> cornerC = { cornerA.x, cornerA.y, cornerH.z };
	Vector3<float> cornerD = { cornerH.x, cornerA.y, cornerH.z };
	Vector3<float> cornerE = { cornerA.x, cornerH.y, cornerA.z };
	Vector3<float> cornerF = { cornerH.x, cornerH.y, cornerA.z };
	Vector3<float> cornerG = { cornerA.x, cornerH.y, cornerH.z };

	Vector2<float> screenPosA = { 0,0 }; if (!ci->worldToScreen(cornerA, screenPosA)) { return false; }
	Vector2<float> screenPosB = { 0,0 }; if (!ci->worldToScreen(cornerB, screenPosB)) { return false; }
	Vector2<float> screenPosC = { 0,0 }; if (!ci->worldToScreen(cornerC, screenPosC)) { return false; }
	Vector2<float> screenPosD = { 0,0 }; if (!ci->worldToScreen(cornerD, screenPosD)) { return false; }
	Vector2<float> screenPosE = { 0,0 }; if (!ci->worldToScreen(cornerE, screenPosE)) { return false; }
	Vector2<float> screenPosF = { 0,0 }; if (!ci->worldToScreen(cornerF, screenPosF)) { return false; }
	Vector2<float> screenPosG = { 0,0 }; if (!ci->worldToScreen(cornerG, screenPosG)) { return false; }
	Vector2<float> screenPosH = { 0,0 }; if (!ci->worldToScreen(cornerH, screenPosH)) { return false; }

	//Bottom face
	drawList->AddLine(screenPosA, screenPosB, espCol, thickness);
	drawList->AddLine(screenPosB, screenPosD, espCol, thickness);
	drawList->AddLine(screenPosC, screenPosD, espCol, thickness);
	drawList->AddLine(screenPosC, screenPosA, espCol, thickness);

	//Top face
	drawList->AddLine(screenPosE, screenPosF, espCol, thickness);
	drawList->AddLine(screenPosF, screenPosH, espCol, thickness);
	drawList->AddLine(screenPosG, screenPosH, espCol, thickness);
	drawList->AddLine(screenPosG, screenPosE, espCol, thickness);

	//Corners to connect bottom and top
	drawList->AddLine(screenPosA, screenPosE, espCol, thickness);
	drawList->AddLine(screenPosB, screenPosF, espCol, thickness);
	drawList->AddLine(screenPosC, screenPosG, espCol, thickness);
	drawList->AddLine(screenPosD, screenPosH, espCol, thickness);

	return true;
}

bool RenderUtils::DrawBlock(const Vector3<int>& blockPos, ImU32 color, float thickness)
{
	AABB<float> box = {
		blockPos,
		blockPos + 1
	};
	return RenderUtils::DrawAABB(box, color, thickness);
}
