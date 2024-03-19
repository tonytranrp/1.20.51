#pragma once

#include "../Module.h"

class RakPeerRunUpdateCycleEvent;
class Blink : public Module
{
private:
	float lastSentTime = 0.f;
	std::shared_ptr<Setting> delay;
	std::shared_ptr<Setting> delayEnabled;
public:
	Blink();
	void onEnable() override;
	void onDisable() override;

	void onRakPeerRunUpdateCycleEvent(RakPeerRunUpdateCycleEvent& event);
};