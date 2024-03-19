#pragma once

#include "MobEvent.h"

class MobQueryJumpPowerEvent : public MobEvent
{
	float power = 0.0f;
public:
	MobQueryJumpPowerEvent(Mob* mob, float power) : MobEvent(mob), power(power) {}
	float getPower() { return this->power; };
	void setPower(float power) { this->power = power; };
};