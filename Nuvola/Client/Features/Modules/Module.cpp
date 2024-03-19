#include "Module.h"

#include "../../Events/EventDispatcher.h"
#include "../../Events/KeyEvent.h"
#include "../../SystemManager.h"
#include "../Keymap/KeymapManager.h"

#include "Render/Notifications.h"

#include "ModuleManager.h"

Module::Module(std::string name, std::string description)  : Module(name, description, 0) {};
Module::Module(std::string name, std::string description, int key) : Manager<Setting>(name) {
	this->wasEnabled = false;
	this->enabled = false;
	this->hotkey = key;
	this->description = description;
}

void Module::onEnable() {

}

void Module::onDisable() {

}

void Module::onTick() {

}

void Module::onHotkeyCheckEvent(KeyEvent& event) {
	if(event.getAction() == KeyAction::PRESSED) {
		if(this->getHotkey() == event.getKey()) {
			this->toggle();
		}
	}
}

void Module::onNuvolaTick() {
	if(enabled) {
		if(!wasEnabled) {
			std::shared_ptr<Notifications> notifs = systems->getModuleManager()->findModule<Notifications>();
			if (notifs != nullptr)
			{
				notifs->notify("Enabled", this->getName(), 2.0f);
			}
			this->onEnable();
		}
		this->onTick();
	} 
	if(!enabled) {
		if(wasEnabled) {
			std::shared_ptr<Notifications> notifs = systems->getModuleManager()->findModule<Notifications>();
			if (notifs != nullptr)
			{
				notifs->notify("Disabled", this->getName(), 2.0f);
			}
			this->onDisable();
		}
	}
	wasEnabled = enabled;
}

auto Module::isEnabled() -> bool {
	return this->enabled;
}

void Module::setEnabled(bool enabled) {
	this->enabled = enabled;
}

void Module::toggle() {
	this->setEnabled(!this->enabled);
}

auto Module::getHotkey() -> int {
	return this->hotkey;
};
void Module::setHotkey(int key, bool save) {
	this->hotkey = key;

	if (save)
	{
		auto keymapMgr = systems->getKeymapManager();
		if (keymapMgr != nullptr)
		{
			const std::shared_ptr<Keymap>& keymap = keymapMgr->get();
			if (keymap)
			{
				keymap->captureFromClient();
				keymapMgr->save(keymap);
			}
		}
	}
};

const std::string& Module::getDescription()
{
	return this->description;
}