#include "Step.h"

#include "../../../Bridge/Components/MaxAutoStepComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

#include "imgui.h"
#include "imgui_custom.h"

Step::Step() : Module("Step", "Increases the max height you can automatically step.") {
    this->height = this->addItem<Setting>("Height", "The maximum height which you will automatically step.", SettingType::SLIDER_FLOAT, 2.f, 0.f, 3.f);
    this->reversestep = this->addItem<Setting>("Reverse-Step", "Reverse of step", SettingType::TOGGLE, false, false, true);
}
bool Step::isKeyDown(std::string key) {
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

bool Step::isKeyDown(int key) {
    return ImGui::IsKeyDown(ImGui::ImGui_ImplWin32_VirtualKeyToImGuiKey(key));
}
void Step::onEnable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<LevelTickEvent, &Step::onLevelTickEvent>(this);
}

void Step::onDisable() {
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;
    player->entityContext.tryGetComponent<MaxAutoStepComponent>()->maxAutoStep = 0.5625f;

    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<LevelTickEvent, &Step::onLevelTickEvent>(this);
}

void Step::onLevelTickEvent(LevelTickEvent& event) {
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;

    if (reversestep->getValueAs<bool>()) {
        auto vel = player->stateVectorComponent->velocity;

        if (vel.y < -0.2f)
            vel.y = -1.f;

        player->stateVectorComponent->velocity = vel;
		return;
    }
    player->entityContext.tryGetComponent<MaxAutoStepComponent>()->maxAutoStep = this->height->getValueAs<float>();
}
