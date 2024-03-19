#pragma once

#include "../Module.h"
#include "../EntityTypeListSetting.h"

class LevelTickEvent;
class RenderEvent;
class ESP : public Module {
	std::shared_ptr<Setting> Player_NameTag;
	std::shared_ptr<Setting> fadeMin;
	std::shared_ptr<Setting> fadeMax;
	std::shared_ptr<EntityTypeListSetting> targets = nullptr;
	std::shared_ptr<Setting> checkTeam;
public:
	ESP();
	void onEnable() override;
	void onDisable() override;

	void onLevelTickEvent(LevelTickEvent& event);
	void onRenderEvent(RenderEvent& event);
};