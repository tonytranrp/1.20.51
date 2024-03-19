#pragma once

#include <Data/Manager.h>
#include "Setting.h"

#include <string>

struct ModeTransitionState
{
	std::shared_ptr<Manager<Setting>> lastActive;
	std::shared_ptr<Manager<Setting>> nowActive;
	float hideLastProg = 0.0f;
	float showActiveProg = 0.0f;

	void update(float deltaTime)
	{
		if (lastActive != nullptr)
		{
			hideLastProg += deltaTime;
			if (hideLastProg > 1.0f)
			{
				hideLastProg = 1.0f;
				lastActive = nullptr;
			}
		}
		else
		{
			showActiveProg += deltaTime;
			if (showActiveProg > 1.0f)
			{
				showActiveProg = 1.0f;
			}
		}
	}
};

class IModeSetting : public Setting, public Manager<Manager<Setting>>
{
	ModeTransitionState transition;
public:
	IModeSetting(std::string name, std::string description, size_t defaultValue, size_t maximumValue, std::vector<std::string> enumStrs = {}) :
		Setting(name, description, SettingType::ENUM, defaultValue, 0, maximumValue, enumStrs), 
		Manager<Manager<Setting>>(name)
	{
		for (std::string& str : enumStrs)
		{
			this->addItem<Manager<Setting>>(str);
		}
		transition.lastActive = nullptr;
		transition.nowActive = this->getItem(0);
	}

	void updateState(float deltaTime)
	{
		std::shared_ptr<Manager<Setting>> activeMgr = this->getItem(this->getCurrentMode());
		if (activeMgr != transition.nowActive)
		{
			transition.hideLastProg = 0.0f;
			transition.showActiveProg = 0.0f;
			transition.lastActive = transition.nowActive;
			transition.nowActive = activeMgr;
		}
		transition.update(deltaTime);
	}

	std::shared_ptr<Manager<Setting>> getManagerFor(size_t mode)
	{
		return this->getItem(mode);
	}

	int getCurrentMode()
	{
		return this->getValueAs<int>();
	}

	const ModeTransitionState& getAnimState()
	{
		return this->transition;
	}
};