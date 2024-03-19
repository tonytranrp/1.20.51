#pragma once

#include "../Module.h"

class Jesus : public Module
{
public:
	Jesus();
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick() override;
};