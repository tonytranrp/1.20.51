#pragma once

#include "../Module.h"

class Test : public Module {
public:
	Test();
	void onEnable() override;
	void onDisable() override;
};