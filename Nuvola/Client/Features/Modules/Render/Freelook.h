#pragma once

#include "../../../Bridge/EntityId.h"
#include "../Module.h"

#include <Math/Math.h>

#include <set>

class Freelook : public Module {
	int lastCameraState = 0;
	Vector2<float> lookingAngles;
	std::set<EntityId> cameras;
	bool hadComponent = false;
public:
	Freelook();
	void onEnable() override;
	void onDisable() override;

	//void onActorRotateEvent(ActorRotateEvent& event) override;
	void onHotkeyCheckEvent(KeyEvent& event) override;
};