#pragma once

#include "../Module.h"

class Actor;
class AntiBot : public Module {
	std::shared_ptr<Setting> mode;
	std::shared_ptr<Setting> IdChecking;
	std::shared_ptr<Setting> ExtraChecks;
public:
	AntiBot();
	void onEnable() override;
	void onDisable() override;

	bool isBot(Actor* actor);
};