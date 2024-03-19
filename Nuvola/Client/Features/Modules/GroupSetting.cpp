#include "GroupSetting.h"

GroupSetting::GroupSetting(std::string name, std::string description) : Setting(name, description, SettingType::GROUP, nullptr, nullptr, nullptr), Manager<Setting>(name) {}

void GroupSetting::updateState(float deltaTime)
{
	if (this->expanded)
	{
		this->expandProg += deltaTime;
		if (this->expandProg > 1.0f)
		{
			this->expandProg = 1.0f;
		}
	}
	else
	{
		this->expandProg -= deltaTime;
		if (this->expandProg < 0.0f)
		{
			this->expandProg = 0.0f;
		}
	}
}
