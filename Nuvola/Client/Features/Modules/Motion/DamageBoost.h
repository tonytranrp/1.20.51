#pragma once

#include "../Module.h"

class ActorMotionEvent;
class DamageBoost : public Module {
	std::shared_ptr<Setting> boostAmount = nullptr;
public:
	DamageBoost();
	void onEnable() override;
	void onDisable() override;
	void onTick() override;
	void onActorMotionEvent(ActorMotionEvent& event);
};
