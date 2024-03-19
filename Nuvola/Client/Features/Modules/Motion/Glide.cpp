#include "Glide.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/IActorMovementProxy.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"
#include "imgui.h"
#include "imgui_custom.h"

#include <Windows.h>

#include <xorstr.hpp>

Glide::Glide() : Module("Glide", "Sets your downward velocity to a small, specific value to make a glide effect.") {
    this->velocity = this->addItem<Setting>("Value", "The downward velocity of the glide.", SettingType::SLIDER_FLOAT, -0.01f, 0.f, -0.15f);
    
}

bool Glide::isKeyDown(std::string key) {
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

bool Glide::isKeyDown(int key) {
    return ImGui::IsKeyDown(ImGui::ImGui_ImplWin32_VirtualKeyToImGuiKey(key));
}

void Glide::onEnable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<LevelTickEvent, &Glide::onLevelTickEvent>(this);
}

void Glide::onDisable() {
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;
    
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<LevelTickEvent, &Glide::onLevelTickEvent>(this);
}

void Glide::onLevelTickEvent(LevelTickEvent& event) {
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;
    player->stateVectorComponent->velocity.y = this->velocity->getValueAs<float>();
    if (isKeyDown("key.jump")) {
        player->stateVectorComponent->velocity.y = 0.42;
    }
}
