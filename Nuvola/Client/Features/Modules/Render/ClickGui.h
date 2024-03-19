#pragma once

#include "../Module.h"

class KeyEvent;
class MouseEvent;
class RenderEvent;
class ClickGui : public Module {
	std::shared_ptr<Setting> scrollSpeed;
public:
	ClickGui();
	void onEnable() override;
	void onDisable() override;
	
	void onRenderEvent(RenderEvent& event);
	void onKeyEvent(KeyEvent& event);
	void onMouseEvent(MouseEvent& event);
};