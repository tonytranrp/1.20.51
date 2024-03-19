#pragma once

#include "Event.h"
#include "Cancellable.h"

enum class KeyAction {
	RELEASED,
	PRESSED
};

class KeyEvent : public Event, public Cancellable {
	int key;
	KeyAction action;
public:
	KeyEvent(int key, int action) : Event() {
		this->key = key;
		this->action = (KeyAction)action;
	};

	[[nodiscard]] int getKey() const {
		return this->key;
	}
	void setKey(int keyId) {
		this->key = keyId;
	}
	KeyAction getAction() {
		return this->action;
	}
	void setAction(KeyAction keyAction) {
		this->action = keyAction;
	}
};
