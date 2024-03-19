#pragma once

#include "Cancellable.h"
#include "LevelEvent.h"

class LevelTickEvent : public LevelEvent, public Cancellable {
public:
	LevelTickEvent(Level* level) : LevelEvent(level) {
	};
};
