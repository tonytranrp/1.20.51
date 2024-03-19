#pragma once

#include "Event.h"
#include "../Bridge/Level.h"

class LevelEvent : public Event {
	Level* level;
public:
	LevelEvent(Level* level) : Event() {
		this->level = level;
	};

	Level* getLevel() {
		return this->level;
	}
};