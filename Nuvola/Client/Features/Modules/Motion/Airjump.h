#pragma once

#include "../Module.h"

class LevelTickEvent;
class Airjump : public Module {
public:
	Airjump();
	void onEnable() override;
	void onDisable() override;
	void onTick() override;
	void onLevelTickEvent(LevelTickEvent& event);
};
