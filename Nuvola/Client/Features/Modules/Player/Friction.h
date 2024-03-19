#pragma once

#include "../Module.h"

class LevelTickEvent;
class Friction : public Module
{
	std::shared_ptr<Setting> x_friction;
	std::shared_ptr<Setting> y_friction;
	std::shared_ptr<Setting> z_friction;
public:
	Friction();

	void onEnable() override;
	void onTick() override;
	void onDisable() override;
	
	void onLevelTickEvent(LevelTickEvent& event);
};