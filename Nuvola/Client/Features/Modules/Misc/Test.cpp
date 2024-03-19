#include "Test.h"

#include "../../../Bridge/Components/MobBodyRotationComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/LoopbackPacketSender.h"
#include "../../../Bridge/ClientInputMappingFactory.h"
#include "../../../Bridge/ClientInputHandler.h"
#include "../../../Bridge/KeyboardMouseSettings.h"
#include "../../../Bridge/Minecraft.h"

Test::Test() : Module("Test", "A module for developer testing.") {
};


void Test::onEnable() {
    const ClientInstance* ci = Bridge::getClientInstance();
    auto* player = ci->getClientPlayer();
};

void Test::onDisable() {
    const ClientInstance* ci = Bridge::getClientInstance();
    auto* player = ci->getClientPlayer();
}

