#pragma once

#include "../Module.h"
#include "../ModeSetting.h"

template<typename T>
class Vector1;

class LevelTickEvent;
class PacketEvent;
class ReceivePacketEvent;
class BHop : public Module
{
	std::shared_ptr<Setting> instantDirection;
	std::shared_ptr<Setting> cutMotion;
	std::shared_ptr<Setting> hop;
	//std::shared_ptr<Setting> impulseCount;
	//std::shared_ptr<Setting> hopHeight;
	std::shared_ptr<Setting> checkLagback;

	int nextLagRandom = 0;
	bool motionCutted = false;
public:
	BHop();
	void onEnable() override;
	void onDisable() override;
	void onTick() override;

	bool isKeyDown(std::string key);
	bool isKeyDown(int key);
	Vector1<float> modYawByInputs(float yaw);
	bool isUsingMoveKeys();

	void onLevelTickEvent(LevelTickEvent& event);
	void onPacketEvent(PacketEvent& event);
	void onReceivePacketEvent(PacketEvent& event);
};