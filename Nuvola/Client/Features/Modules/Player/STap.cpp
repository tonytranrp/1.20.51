#include "STap.h"

#include "../../../../Utils/TargetUtils.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/Options.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

#include "../../../Patches/Global/KeyPressHook.h"

#include <imgui.h>
#include <imgui_custom.h>

STap::STap() : Module("S-Tap", "Automatically presses the walk backwards button to keep distance from opponents in combat.")
{
	//A setting for our range
	this->range = this->addItem<Setting>("Range", "The range at which the walk backwards button should be automatically pressed.", SettingType::SLIDER_FLOAT, 3.0f, 0.0f, 10.0f);
	this->targets = this->addItem<EntityTypeListSetting>("Targets", "Which entities should or shouldn't be targeted.");
	//Setting to check if teammates should be ignored
	this->checkTeams = this->addItem<Setting>("Check Teams", "If teammates should be ignored.", SettingType::TOGGLE, false, false, true);
}

static bool stapping = false;
static bool stop = false;
static Actor* theTarget = nullptr;

void STap::onEnable()
{
	theTarget = nullptr;
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (!player) return;

	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &STap::onLevelTickEvent>(this);
}

void STap::onDisable()
{
	theTarget = nullptr;
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (!player) return;
	ClientInputHandler* cih = ci->mClientInputHandler;
	if (!cih) return;
	ClientInputMappingFactory* cimf = cih->factory;
	if (!cimf) return;
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	if (!kms) return;

	int backKey = kms->GetMCKey("key.back");

	stapping = false;
	stop = false;
	KeyPressHook::createPress(backKey, KeyAction::RELEASED);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &STap::onLevelTickEvent>(this);
}

void STap::onTick()
{
	
}

void STap::onLevelTickEvent(LevelTickEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (!player) return;
	ClientInputHandler* cih = ci->mClientInputHandler;
	if (!cih) return;
	ClientInputMappingFactory* cimf = cih->factory;
	if (!cimf) return;
	KeyboardMouseSettings* kms = cimf->keyboardMouseControls;
	if (!kms) return;

	Actor* theTarget = TargetUtils::findTarget(player,
		this->targets->getSelectedTypes(),
		range->getValueAs<float>(),
		false,
		checkTeams->getValueAs<bool>()
	);
	if (theTarget != nullptr) {
		//If the target is still in range, keep it
		if (theTarget->getPosition().distanceTo(player->getPosition()) > range->getValueAs<float>()) {
			theTarget = nullptr;
		}
	};

	int backKey = kms->GetMCKey("key.back");

	//We found the backwards movement key
	if (backKey != 0 && theTarget) {
		if (!stapping) {
			KeyPressHook::createPress(backKey, KeyAction::PRESSED);
			stapping = true;
		}
	}

	if (!theTarget && !stop) {
		ImGuiKey imkey = ImGui::ImGui_ImplWin32_VirtualKeyToImGuiKey(backKey);
		stop = !ImGui::IsKeyDown(imkey);
	}

	if (stop) {
		KeyPressHook::createPress(backKey, KeyAction::RELEASED);
		stapping = false;
		stop = false;
	}
}