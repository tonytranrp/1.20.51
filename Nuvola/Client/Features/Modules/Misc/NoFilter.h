#pragma once

#include "../Module.h"

class NoFilter : public Module
{
	bool wasEnabled = false;
public:
	NoFilter();
	void onEnable() override;
	void onDisable() override;
};