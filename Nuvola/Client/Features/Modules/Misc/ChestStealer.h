#pragma once

#include "../Module.h"

class ContainerScreenControllerTickEvent;
class ChestStealer : public Module
{
	std::shared_ptr<Setting> delay;
public:
	ChestStealer();

	void onEnable() override;
	void onDisable() override;

	void onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event);
};