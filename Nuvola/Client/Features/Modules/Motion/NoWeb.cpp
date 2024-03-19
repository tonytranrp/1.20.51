#include "NoWeb.h"

#include "../../../Bridge/Components/BlockMovementSlowdownMultiplierComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

NoWeb::NoWeb() : Module("NoWeb", "Prevents cobwebs from slowing you down.") {}

void NoWeb::onEnable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->listen<LevelTickEvent, &NoWeb::onLevelTickEvent>(this);
}

void NoWeb::onDisable() {
    auto dispatcher = systems->getEventDispatcher();
    dispatcher->deafen<LevelTickEvent, &NoWeb::onLevelTickEvent>(this);
}

void NoWeb::onLevelTickEvent(LevelTickEvent& event) {
    ClientInstance* ci = Bridge::getClientInstance();
    if (ci == nullptr) return;
    LocalPlayer* player = ci->getClientPlayer();
    if (player == nullptr) return;

    player->entityContext.tryGetComponent<BlockMovementSlowdownMultiplierComponent>()->blockMovementSlowdownMultiplier = Vector3<float>(0.f, 0.f, 0.f);
}