#pragma once

#include "../Module.h"

class LevelTickEvent;
class Spider : public Module
{
	std::shared_ptr<Setting> speed = nullptr;
public:
	Spider();
	void onEnable() override;
	void onDisable() override;

	void onLevelTickEvent(LevelTickEvent& event);
};