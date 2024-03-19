#include "AutoSprint.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "../../../Events/PacketEvent.h"

#include <imgui.h>
#include <imgui_custom.h>

AutoSprint::AutoSprint() : Module("AutoSprint", "Makes the player sprint automatically when moving.") {
	multidirectional = this->addItem<Setting>("Multidirectional", "Allows the player to sprint in all directions, not just forward.", SettingType::TOGGLE, true, false, true);
}

void AutoSprint::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &AutoSprint::onLevelTickEvent>(this);
	dispatcher->listen<PacketEvent, &AutoSprint::onPacketEvent>(this);
}

void AutoSprint::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &AutoSprint::onLevelTickEvent>(this);
	dispatcher->deafen<PacketEvent, &AutoSprint::onPacketEvent>(this);
}

bool AutoSprint::isKeyDown(std::string key) {
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return false;
	ClientInputHandler* cih = ci->mClientInputHandler;
	if (!cih) return false;
	ClientInputMappingFactory* cimf = cih->factory;
	if (!cimf) return false;
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	if (!kms) return false;


	return isKeyDown(kms->GetMCKey(std::move(key)));
}

bool AutoSprint::isKeyDown(int key) {
	return ImGui::IsKeyDown(ImGui::ImGui_ImplWin32_VirtualKeyToImGuiKey(key));
}

void AutoSprint::onLevelTickEvent(LevelTickEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	LocalPlayer* lp = ci->getClientPlayer();
	if (!lp) return;
	ClientInputHandler* cih = ci->mClientInputHandler;
	if (!cih) return;
	ClientInputMappingFactory* cimf = cih->factory;
	if (!cimf) return;
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	if (!kms) return;

	bool w = isKeyDown("key.forward");
	bool a = isKeyDown("key.left");
	bool s = isKeyDown("key.back");
	bool d = isKeyDown("key.right");
	bool moveDown = w || a || s || d;
	if (w || (multidirectional->getValueAs<bool>() && moveDown))
	{
		lp->setSprinting(true);
	}
}

void AutoSprint::onPacketEvent(PacketEvent& event)
{
	if (event.getPacket()->getId() == PacketID::PlayerAuthInputPacket)
	{
		auto* paip = (PlayerAuthInputPacket*)event.getPacket();
		paip->inputData |= AuthInputAction::SPRINT_DOWN;
		paip->inputData |= AuthInputAction::SPRINTING;
	}
}
