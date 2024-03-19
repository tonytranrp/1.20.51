#pragma once

#include "../Module.h"

class MobQueryJumpPowerEvent;
class HighJump : public Module
{
	std::shared_ptr<Setting> jumpPower = nullptr;
	bool needsJump = false;

public:
	HighJump();
	void onEnable() override;
	void onDisable() override;
	void onMobQueryJumpPowerEvent(MobQueryJumpPowerEvent& event);
};