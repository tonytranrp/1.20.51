#pragma once

#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"

#include "Screen.h"

class ClientScreen : public Screen
{
public:
	ClientScreen(std::string name) : Screen(name) {};
	virtual void handleInput(KeyEvent& event) = 0;
	virtual void handleInput(MouseEvent& event) = 0;
};