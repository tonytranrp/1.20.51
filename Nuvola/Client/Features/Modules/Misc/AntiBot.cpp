#include "AntiBot.h"

#include "../../../Bridge/Actor.h"

#include <xorstr.hpp>

enum class AntiBotMode_t { SIZE, HITBOX , ADVANCED};
static std::vector<std::string> antibotModes = { "Size", "Hitbox" , "Advanced"};

AntiBot::AntiBot() : Module(xorstr_("AntiBot"), xorstr_("Prevents modules from interacting with bots."))
{
	//this->setEnabled(true);
	this->mode = this->addItem<Setting>(xorstr_("Mode"), xorstr_("The method in which a bot should be determined."), SettingType::ENUM, 0, 0, (int)antibotModes.size(), antibotModes);
	this->IdChecking = this->addItem<Setting>("Entity Id Check", "Self Explanitory", SettingType::TOGGLE, false, false, true);
	this->ExtraChecks = this->addItem<Setting>("Extra Checks", "Looks for more things to flag as a bot", SettingType::TOGGLE, false, false, true);
}

void AntiBot::onEnable()
{
}

void AntiBot::onDisable()
{
}

bool AntiBot::isBot(Actor* actor)
{
	if (!this->isEnabled()) return false;

	AntiBotMode_t mode = (AntiBotMode_t)this->mode->getValueAs<int>();
	switch (mode) {
	case AntiBotMode_t::SIZE: {
		if (actor->aabbComponent->width == 0.6f && actor->aabbComponent->height == 1.8f) {
			return false;
		}
		return true;
	}

	case AntiBotMode_t::ADVANCED: {
		bool CheckTwo = false;
		bool CheckThree = false;
		if (this->ExtraChecks->getValueAs<bool>() && actor->isImmobile() || actor->getNameTag().length() < 1 || std::string(actor->getNameTag()).find(std::string("\n")) != std::string::npos) {
			CheckTwo = true;
		}
		if (actor->aabbComponent->width == 0.6f && actor->aabbComponent->height == 1.8f) {
			CheckThree = true;
		}
		if (CheckTwo || CheckThree) {
			return false;
		}
		else
		{
			return true;
		}
	
	}
	

	case AntiBotMode_t::HITBOX:
	default: {
		const AABB<>& hitbox = actor->aabbComponent->aabb;
		float width = hitbox.secondCorner.x - hitbox.firstCorner.x;
		float height = hitbox.secondCorner.y - hitbox.firstCorner.y;
		float length = hitbox.secondCorner.z - hitbox.firstCorner.z;

		if ((width < 0.7f && width > 0.5f) && (length < 0.7f && length > 0.5f) && (height < 1.9f && height > 1.7f)) {
			return false;
		}
		return true;
	}
	}

	return true;
}
