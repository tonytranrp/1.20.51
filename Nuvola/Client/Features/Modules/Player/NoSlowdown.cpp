#include "NoSlowdown.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/PlayerSlowedByItemEvent.h"

NoSlowdown::NoSlowdown() : Module("NoSlowdown", "Prevents item use from slowing down the player.") {}

void NoSlowdown::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<PlayerSlowedByItemEvent, &NoSlowdown::onPlayerSlowedByItemEvent>(this);
}
void NoSlowdown::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<PlayerSlowedByItemEvent, &NoSlowdown::onPlayerSlowedByItemEvent>(this);
}

void NoSlowdown::onPlayerSlowedByItemEvent(PlayerSlowedByItemEvent& event)
{
	event.setCancelled(true);
}