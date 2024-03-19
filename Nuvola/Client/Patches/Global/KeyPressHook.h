#pragma once

#include "../../Events/KeyEvent.h"
#include "../IPatch.h"

class KeyPressHook : public IPatch {
	static auto keyPressCallback_1_17_10_4(int key, int action) -> void;
public:
	static inline uintptr_t funcOriginal = 0;
	KeyPressHook();
	auto apply() -> bool override;

	static void createPress(int key, KeyAction action);
};
