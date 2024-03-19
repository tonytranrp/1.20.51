#pragma once

#include "../Module.h"

class LevelTickEvent;
class Timer : public Module {
	std::shared_ptr<Setting> tps = nullptr;
public:
	Timer();
	void onEnable() override;
	void onDisable() override;
	void onTick() override;
	void onLevelTickEvent(LevelTickEvent& event);
};
