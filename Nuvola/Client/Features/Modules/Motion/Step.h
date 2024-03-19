#pragma once

#include "../Module.h"

class LevelTickEvent;
class Step : public Module {
public:
	std::shared_ptr<Setting> height = nullptr;
	std::shared_ptr<Setting> reversestep;

	Step();
	void onEnable() override;
	void onDisable() override;
	void onLevelTickEvent(LevelTickEvent& event);
	bool isKeyDown(int key);
	bool isKeyDown(std::string key);
	//void onActorMotionEvent(ActorMotionEvent& event) override;
};