#pragma once

#include "../Module.h"

#include <string>

class RenderEvent;
class Notifications : public Module
{
public:
	Notifications();
	void notify(std::string message, std::string title = "Nuvola", float time = 5.0f);

	void onRenderEvent(RenderEvent&);
};