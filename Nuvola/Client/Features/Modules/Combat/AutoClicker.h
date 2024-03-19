#pragma once

#include "../Module.h"

class LevelTickEvent;
class AutoClicker : public Module {
	std::shared_ptr<Setting> leftClick;
	std::shared_ptr<Setting> rightClick;
	std::shared_ptr<Setting> held;
	//std::shared_ptr<Setting> perfectHits;
	std::shared_ptr<Setting> interval;
	std::shared_ptr<Setting> clickInMenu;
	std::shared_ptr<Setting> simulateReal;

	float lastAttackTime = 0.0f;
public:
	AutoClicker();
	virtual ~AutoClicker();

	void onEnable() override;
	void onDisable() override;

	void onLevelTickEvent(LevelTickEvent& event);
};