#include "Phase.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/LevelTickEvent.h"

Phase::Phase() : Module("Phase", "Allows you to noclip through blocks.")
{}

void Phase::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<LevelTickEvent, &Phase::onLevelTickEvent>(this);
}

void Phase::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<LevelTickEvent, &Phase::onLevelTickEvent>(this);

	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* lp = ci->getClientPlayer();
	if (lp == nullptr) return;
	AABBShapeComponent* component = lp->aabbComponent;
	if (component == nullptr) return;

	component->aabb.secondCorner.y += 1.8f;
}

void Phase::onLevelTickEvent(LevelTickEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* lp = ci->getClientPlayer();
	if (lp == nullptr) return;
	AABBShapeComponent* component = lp->aabbComponent;
	if (component == nullptr) return;
	component->aabb.secondCorner.y = component->aabb.firstCorner.y;
}