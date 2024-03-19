#include "Jesus.h"

#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Events/EventDispatcher.h"

Jesus::Jesus() : Module("Jesus", "Walk on water, just like christ")
{
}

void Jesus::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	//dispatcher->sink<RenderEvent>().connect<&ModList::onRenderEvent>(*this);
}

void Jesus::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	//dispatcher->sink<RenderEvent>().connect<&ModList::onRenderEvent>(*this);
}

void Jesus::onTick()
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;
	
	if (player->isInWater()) {
		player->stateVectorComponent->velocity.y = 0.06f;
		player->setOnGround(true);
	}
}