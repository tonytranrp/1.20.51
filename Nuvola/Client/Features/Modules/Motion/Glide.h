#pragma once

#include "../Module.h"

class LevelTickEvent;
class Glide : public Module {
	std::shared_ptr<Setting> velocity = nullptr;
public:
	Glide();
	void onEnable() override;
	void onDisable() override;
	void onLevelTickEvent(LevelTickEvent& event);
	bool isKeyDown(int key);
	bool isKeyDown(std::string key);
};