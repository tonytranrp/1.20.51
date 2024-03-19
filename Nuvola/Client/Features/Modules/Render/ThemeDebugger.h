#pragma once

#include "../Module.h"

class RenderEvent;
class ThemeDebugger : public Module
{
public:
	ThemeDebugger();

	void onRenderEvent(RenderEvent& event);
};