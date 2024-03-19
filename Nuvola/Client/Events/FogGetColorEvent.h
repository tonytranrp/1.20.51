#pragma once

#include "Event.h"

class FogGetColorEvent : public Event {
	ColorF color;
public:
	FogGetColorEvent(const ColorF& defaultColor) : Event() {
		this->color = defaultColor;
	};

	const ColorF& getColor() {
		return this->color;
	}

	void setColor(const ColorF& c) {
		this->color = c;
	}
};