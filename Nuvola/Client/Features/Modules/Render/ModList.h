#pragma once

#include "../Module.h"

class RenderEvent;
class ModList : public Module {
public:
	ModList();

	void onEnable() override;
	void onDisable() override;
	
	void onRenderEvent(RenderEvent& event);
};
