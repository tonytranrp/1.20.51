#pragma once

#include "../Module.h"
#include "../ModeSetting.h"

enum class autoTotemModes_t { Legit, Transactions };
class ContainerScreenControllerTickEvent;
class LevelTickEvent;
class AutoTotem : public Module
{
protected:
	std::shared_ptr<ModeSetting<autoTotemModes_t>> mode;
	std::shared_ptr<Setting> delay;

public:
	AutoTotem();

	void onEnable() override;
	void onDisable() override;

	void onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event);
	void onLevelTickEvent(LevelTickEvent& event);
};