#include "Airjump.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

Airjump::Airjump() : Module("Airjump", "Allows the player to jump on air.") {

}
void Airjump::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &Airjump::onLevelTickEvent>(this);
}
void Airjump::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &Airjump::onLevelTickEvent>(this);
}
void Airjump::onTick() {
	
}

void Airjump::onLevelTickEvent(LevelTickEvent& event)
{
	ClientInstance* client = Bridge::getClientInstance();
	if (client) {
		LocalPlayer* player = client->getClientPlayer();
		if (player) {
			player->setOnGround(true);
		}
	}
}
