#pragma once

#include "../Bridge/ScreenController.h"

class ScreenControllerEvent
{
	ScreenController* controller;
public:
	explicit ScreenControllerEvent(ScreenController* controller)
	{
		this->controller = controller;
	}

	ScreenController* getController()
	{
		return this->controller;
	}
};