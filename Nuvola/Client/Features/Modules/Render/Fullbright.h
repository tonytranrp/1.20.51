#pragma once

#include "../Module.h"

class LevelTickEvent;
class Fullbright : public Module
{
	std::shared_ptr<Setting> modeSetting;
public:
	Fullbright();
	void onEnable() override;
	void onDisable() override;

	void onLevelTickEvent(LevelTickEvent&);
};