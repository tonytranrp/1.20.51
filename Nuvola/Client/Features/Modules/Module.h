#pragma once

#include "Setting.h"

#include <Data/Manager.h>

#include <iostream>
#include <string>

#define CHECK(pointer) if(pointer == nullptr) return;

class KeyEvent;
class Module : public Manager<Setting> {
	bool enabled;
	bool wasEnabled;
	int hotkey;
	std::string description;
public:
    Module(std::string name, std::string description);
    Module(std::string name, std::string description, int hotkey);

    virtual void onEnable();
    virtual void onDisable();
	virtual void onTick();
	virtual void onHotkeyCheckEvent(KeyEvent& event);
	void onNuvolaTick();
	auto isEnabled() -> bool;
	void setEnabled(bool enabled);
	void toggle();
	auto getHotkey() -> int;
	void setHotkey(int key, bool save = true);
	const std::string& getDescription();
};
