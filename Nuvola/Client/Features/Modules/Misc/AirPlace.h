#pragma once

#include "../Module.h"

#include <Math/Math.h>

class LevelTickEvent;
class MouseEvent;
class KeyEvent;
class RenderEvent;
class AirPlace : public Module {
	bool needsPlace = false;
	Vector3<int> blockPos;
public:
	AirPlace();
	void onEnable() override;
	void onDisable() override;

	void onLevelTick(LevelTickEvent& event);
	void onMouseEvent(MouseEvent& event);
	void onKeyEvent(KeyEvent& event);
	void onRenderEvent(RenderEvent& event);
};