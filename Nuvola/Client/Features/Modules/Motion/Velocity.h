#pragma once

#include "../Module.h"

class ActorMotionEvent;
class Velocity : public Module
{
	std::shared_ptr<Setting> velocityXZ;
	std::shared_ptr<Setting> velocityY;
public:
	Velocity();
	void onEnable() override;
	void onDisable() override;

	void onActorMotionEvent(ActorMotionEvent& event);
};