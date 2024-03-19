#include "AutoClicker.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/MinecraftGame.h"
#include "../../../Bridge/Minecraft.h"
#include "../../../Patches/Global/MouseActionHook.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../Render/ClickGui.h"
#include "../ModuleManager.h"

#include <imgui.h>
#include <thread>
#include <xorstr.hpp>

AutoClicker::AutoClicker() : Module(xorstr_("AutoClicker"), xorstr_("Automatically clicks for you."))
{
	//A setting to left click
	this->leftClick = this->addItem<Setting>(xorstr_("Left"), xorstr_("Enable to autoclick on the left click button."), SettingType::TOGGLE, true, false, true);
	//A setting to right click
	this->rightClick = this->addItem<Setting>(xorstr_("Right"), xorstr_("Enable to autoclick on the right click button."), SettingType::TOGGLE, false, false, true);
	//A setting for if the real button needs to be down
	this->held = this->addItem<Setting>(xorstr_("Held"), xorstr_("If the mouse button should be down for the autoclick to function."), SettingType::TOGGLE, true, false, true);
	//A setting to perfectly time hits
	//this->perfectHits = this->addItem<Setting>("Perfect Hits", SettingType::TOGGLE, false, false, true);
	//A setting for the max attacks per second
	this->interval = this->addItem<Setting>(xorstr_("Interval"), xorstr_("The number of clicks per second."), SettingType::SLIDER_FLOAT, 10.0f, 0.0f, 20.0f);
	//A setting to decide if we can autoclick in menus or not
	//this->clickInMenu = this->addItem<Setting>(xorstr_("Click in Menus"), xorstr_("If the autoclicker should be active while GUIs are open."), SettingType::TOGGLE, false, false, true);
	//A setting to decide if a real mouse click will be simulated, or if it should just hit the target
	//this->simulateReal = this->addItem<Setting>(xorstr_("Simulate Mouse"), xorstr_("Makes the autoclicker call the mouse click function rather than faking a click."), SettingType::TOGGLE, true, false, true);
}

AutoClicker::~AutoClicker()
{
}

void AutoClicker::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &AutoClicker::onLevelTickEvent>(this);
}

void AutoClicker::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &AutoClicker::onLevelTickEvent>(this);
}

void AutoClicker::onLevelTickEvent(LevelTickEvent& event)
{
	//We only want to attack or target when the mod is enabled
	if (!this->isEnabled()) return;

	//Get game vars we need
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	if (!ci->isInGame()) return;
	Minecraft* mc = ci->mMinecraft;
	if (!mc) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (!player) return;
	MinecraftGame* mcg = ci->mMinecraftGame;
	if (!mcg) return;
	if (!mcg->canInputMovement) return;

	std::shared_ptr<ClickGui> gui = systems->getModuleManager()->findModule<ClickGui>();
	if (gui->isEnabled()) return;

	/*if (perfectHits->getValueAs<bool>()) {
		if (player->isLookingAtAnEntity()) {
			
		}
	}*/

	bool canAttack = mc->tickTimer->seconds - this->lastAttackTime >= 1.0f / this->interval->getValueAs<float>();

	static std::vector<MouseButton> clickButtons;
	if (this->leftClick->getValueAs<bool>()) {
		if (this->held->getValueAs<bool>() && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		}
		else {
			clickButtons.push_back(MouseButton::LEFT);
		}
	}
	if (this->rightClick->getValueAs<bool>()) {
		if (this->held->getValueAs<bool>() && !ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
		}
		else {
			clickButtons.push_back(MouseButton::RIGHT);
		}
	}

	for (MouseButton mb : clickButtons) {
		if (canAttack) {
			MouseActionHook::createClick(mb, MouseAction::PRESS);
			MouseActionHook::createClick(mb, MouseAction::RELEASE);
			this->lastAttackTime = mc->tickTimer->seconds;
		}
	}
	clickButtons.clear();
}
