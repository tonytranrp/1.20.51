#pragma once

#include <Math/Math.h>
#include "Event.h"
#include "Cancellable.h"

enum class MouseButton {
	NONE = 0,
	LEFT = 1,
	RIGHT = 2,
	MMB = 3,
	SCROLL = 4
};
enum class MouseAction {
	PRESS,
	RELEASE,
	SCROLL_UP,
	SCROLL_DOWN
};

class MouseEvent : public Event, public Cancellable {
	MouseButton button;
	MouseAction action;
	short mouseX;
	short mouseY;
	short movementX;
	short movementY;
public:
	MouseEvent(char button, uint8_t action, short mouseX, short mouseY, short movementX, short movementY) : Event() {
		this->button = (MouseButton)button;
		this->setActionFromChar(action);
		this->mouseX = mouseX;
		this->mouseY = mouseY;
		this->movementX = movementX;
		this->movementY = movementY;
	};

	MouseButton getButton() {
		return this->button;
	}
	void setButton(MouseButton button) {
		this->button = button;
	}
	MouseAction getAction() {
		return this->action;
	}
	char getActionAsChar() {
		switch(this->action) {
			case MouseAction::RELEASE: return 0;
			case MouseAction::PRESS: return 1;
			case MouseAction::SCROLL_UP: return 0x78;
			case MouseAction::SCROLL_DOWN: return 0x80;
			default: return (char)this->action;
		}
	}
	void setAction(MouseAction action) {
		this->action = action;
	}
	void setActionFromChar(uint8_t action) {
		switch(action) {
			case 0: this->action = MouseAction::RELEASE; break;
			case 1: this->action = MouseAction::PRESS; break;
			case 0x78: this->action = MouseAction::SCROLL_UP; break;
			case 0x7F: this->action = MouseAction::SCROLL_UP; break;
			case 0x80: this->action = MouseAction::SCROLL_DOWN; break;
			case 0x88: this->action = MouseAction::SCROLL_DOWN; break;
			default: this->action = (MouseAction)action; break;
		}
	}
	
	short getMouseX() {
		return this->mouseX;
	}
	void setMouseX(short mouseX) {
		this->mouseX = mouseX;
	}
	short getMouseY() {
		return this->mouseY;
	}
	void setMouseY(short mouseY) {
		this->mouseY = mouseY;
	}
	Vector2<short> getMousePos() {
		Vector2<short> pos = Vector2<short>(this->getMouseX(), this->getMouseY());
		return pos;
	}
	Vector2<float> getMousePosF() {
		Vector2<float> posF = Vector2<float>((float)this->getMouseX(), (float)this->getMouseY());
		return posF;
	}
};