#pragma once

#include "../Module.h"

class MouseEvent;
class RenderEvent;
class ClickTp : public Module {
	std::shared_ptr<Setting> range = nullptr;
	bool needsTp = false;
public:
	ClickTp();
	void onEnable() override;
	void onDisable() override;
	void onMouseEvent(MouseEvent& event);
	void onRenderEvent(RenderEvent& event);
};
