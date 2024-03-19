#pragma once

#include "../Module.h"

class Reach : public Module
{
	std::shared_ptr<Setting> range = nullptr;
public:
	Reach();
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick() override;
};