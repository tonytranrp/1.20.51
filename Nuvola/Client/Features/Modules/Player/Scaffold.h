#pragma once

#include "../Module.h"

template<typename T>
struct Vector3;

class LevelTickEvent;
class PacketEvent;
class RenderEvent;
class Scaffold : public Module
{
	std::shared_ptr<Setting> buildSides;
	std::shared_ptr<Setting> instantExtend;
	std::shared_ptr<Setting> extend;
    std::shared_ptr<Setting> distance;
    std::shared_ptr<Setting> places;
public:
	Scaffold();

	void onEnable() override;
	void onDisable() override;

	void onLevelTickEvent(LevelTickEvent& event);
	void onPacketEvent(PacketEvent& event);
	void onRenderEvent(RenderEvent& event);

    Vector3<int> getClosestPlacePos(Vector3<int> pos, int maxDistance = 4);

    int getBlockPlaceFace(Vector3<int> pos);

    bool isValidPlacePos(Vector3<int> pos);

    Vector3<int> getSideOffset(int side);

    int getClosestBlockFace(Vector3<int> blockSel);

    Vector3<int> getBlockPosRotation(float extend);

    bool isAirBlock(Vector3<int> bp);

    void placeBlock(Vector3<int>, int side);

    std::vector<Vector3<int>> getBlocksBetween(Vector3<int> start, Vector3<int> end, bool includeAir);

    bool isKeyDown(int key);
};