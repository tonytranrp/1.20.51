#pragma once

#include "../../Events/MouseEvent.h"
#include "../IPatch.h"

class MouseActionHook : public IPatch {
	static auto mouseActionHookCallback_1_17_11_1(void* param_1,char button, char action, short mouseX, short mouseY, short movementX, short movementY, bool param_8) -> void;
public:
	static inline uintptr_t funcOriginal = 0;
	MouseActionHook();
	auto apply() -> bool override;

	static void createClick(MouseButton button, MouseAction action);
};
