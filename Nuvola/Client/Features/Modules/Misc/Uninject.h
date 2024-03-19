#pragma once

#include "../Module.h"

class Uninject : public Module {
public:
	Uninject();
	void onEnable() override;
	void onDisable() override;
};