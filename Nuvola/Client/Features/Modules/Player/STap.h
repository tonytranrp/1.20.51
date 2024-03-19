#pragma once

#include "../Module.h"
#include "../EntityTypeListSetting.h"

class LevelTickEvent;
class STap : public Module {
	std::shared_ptr<Setting> range;
	std::shared_ptr<EntityTypeListSetting> targets = nullptr;
	std::shared_ptr<Setting> checkTeams;

public:
	STap();

	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick() override;

	void onLevelTickEvent(LevelTickEvent& event);
};