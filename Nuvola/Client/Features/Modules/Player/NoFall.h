#pragma once

#include "../Module.h"

class LevelTickEvent;
class PacketEvent;
class NoFall : public Module {
	std::shared_ptr<Setting> mode = nullptr;
public:
	NoFall();
	void onEnable() override;
	void onDisable() override;
	void onTick() override;

	void onLevelTickEvent(LevelTickEvent& event);
    void onPacketEvent(PacketEvent& event);
};