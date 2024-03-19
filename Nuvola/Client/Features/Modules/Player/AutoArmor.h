#pragma once

#include "../Module.h"
#include "../EnchantWeightSetting.h"

class ContainerScreenControllerTickEvent;
class LevelTickEvent;
class AutoArmor : public Module
{
protected:
	std::shared_ptr<Setting> mode;
	std::shared_ptr<Setting> delay;
	std::shared_ptr<Setting> discardUseless;
	std::shared_ptr<EnchantWeightSetting> weightsSetting;

	bool running = false;
public:
	AutoArmor();

	void onEnable() override;
	void onDisable() override;

	bool isRunning() const;
	float getArmorValue(const class ItemStackBase* stack) const;

	void onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event);
	void onLevelTickEvent(LevelTickEvent& event);
};