#pragma once

#include "../Module.h"

class LevelTickEvent;
class Phase : public Module
{
public:
	Phase();
	void onEnable() override;
	void onDisable() override;

	void onLevelTickEvent(LevelTickEvent& event);
};