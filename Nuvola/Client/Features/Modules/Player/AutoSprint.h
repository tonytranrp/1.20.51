#pragma once

#include "../Module.h"

class LevelTickEvent;
class PacketEvent;
class AutoSprint : public Module {
	std::shared_ptr<Setting> multidirectional = nullptr;
public:
	AutoSprint();
	void onEnable() override;
	void onDisable() override;

	bool isKeyDown(std::string key);
	bool isKeyDown(int key);

	void onLevelTickEvent(LevelTickEvent& event);
	void onPacketEvent(PacketEvent& event);
};