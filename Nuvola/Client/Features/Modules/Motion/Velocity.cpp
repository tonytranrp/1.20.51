#include "Velocity.h"

#include "../../../Bridge/Components/ActorMovementProxyComponent.h"
#include "../../../Bridge/Components/MobHurtTimeComponent.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Events/ActorMotionEvent.h"
#include "../../../Events/EventDispatcher.h"

Velocity::Velocity() : Module("Velocity", "Modifies your knockback.")
{
	this->velocityXZ = this->addItem<Setting>("XZ Velocity", "Percentage of knockback you will take on the X and Z axis", SettingType::SLIDER_FLOAT, 0.0f, 0.0f, 100.0f);
	this->velocityY = this->addItem<Setting>("Y Velocity", "Percentage of knockback you will take on the Y axis.", SettingType::SLIDER_FLOAT, 0.0f, 0.0f, 100.0f);
}

void Velocity::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ActorMotionEvent, &Velocity::onActorMotionEvent>(this);
}

void Velocity::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<ActorMotionEvent, &Velocity::onActorMotionEvent>(this);
}

void Velocity::onActorMotionEvent(ActorMotionEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (!ci) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (!player) return;
	if (event.getActor() != player) return;

	float percentXZ = velocityXZ->getValueAs<float>();
	float percentY = velocityY->getValueAs<float>();

	Vector3<float>* motion = event.getMotion();
	motion->x *= percentXZ / 100.0f;
	motion->y *= percentY / 100.0f;
	motion->z *= percentXZ / 100.0f;
	event.setMotion(*motion);
}
