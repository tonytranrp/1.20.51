#pragma once

#include "../Module.h"

class LevelTickEvent;
class NoWeb : public Module {
public:
	NoWeb();
	void onEnable() override;
	void onDisable() override;
	void onLevelTickEvent(LevelTickEvent& event);
};