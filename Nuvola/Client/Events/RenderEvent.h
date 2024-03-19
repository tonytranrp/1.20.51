#pragma once

#include "Event.h"

enum class RenderTime {
	WORLD,
	UNDERLAY,
	MENU,
	OVERLAY
};

class RenderEvent : public Event {
	RenderTime currentTime;
public:
	RenderEvent(RenderTime time) : Event() {
		this->currentTime = time;
	};

	RenderTime getTime() {
		return this->currentTime;
	}
};