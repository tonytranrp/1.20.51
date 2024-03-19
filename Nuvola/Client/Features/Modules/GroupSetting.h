#pragma once

#include "Setting.h"

#include <Data/Manager.h>

class GroupSetting : public Setting, public Manager<Setting>
{
public:
	float expandProg = 0.0f;
	bool expanded = false;
	GroupSetting(std::string name, std::string description);
	virtual void updateState(float deltaTime);
};