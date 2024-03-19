#pragma once

#include "Event.h"

class GetCloudHeightEvent : public Event {
	int height;
public:
	GetCloudHeightEvent(const int defaultHeight) : Event() {
		this->height = defaultHeight;
	};

	const int getHeight() {
		return this->height;
	}

	void setHeight(const int h) {
		this->height = h;
	}
};