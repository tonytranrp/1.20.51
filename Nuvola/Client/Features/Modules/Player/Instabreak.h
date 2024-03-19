#pragma once

#include "../Module.h"

class KeyEvent;
class LevelTickEvent;
class MouseEvent;
class Instabreak : public Module
{
	bool needsBreak = false;
public:
	Instabreak();

	void onEnable() override;
	void onDisable() override;

	void onMouseEvent(MouseEvent& event);
	void onKeyEvent(KeyEvent& event);
	void onLevelTickEvent(LevelTickEvent& event);
	//void onRenderEvent(RenderEvent& event) override;
};