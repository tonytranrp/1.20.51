#pragma once

#include "../Module.h"
#include "../ModeSetting.h"

template<typename T>
class Vector2;

enum class flyMode_t
{
	Jetpack,
	Vanilla,
	Motion
	
};

class LevelTickEvent;
class Fly : public Module {
	std::shared_ptr<ModeSetting<flyMode_t>> mode;
	std::shared_ptr<Setting> speed;
	std::shared_ptr<Setting> speedy;
	std::shared_ptr<Setting> cutMotion;
public:
	Fly();
	void onEnable() override;
	void onDisable() override;
	void onTick() override;
	void onHotkeyCheckEvent(KeyEvent& event) override;

	void onLevelTickEvent(LevelTickEvent& event);

    Vector2<float> getMotion(float playerYaw, float speed);

    bool isKeyDown(int key);
	bool isKeyDown(std::string key);
private:
	bool canWrite = false, lastState = false;
};