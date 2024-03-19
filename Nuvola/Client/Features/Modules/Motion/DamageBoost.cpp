#include "DamageBoost.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"

#include "../../../Events/ActorMotionEvent.h"
#include "../../../Events/EventDispatcher.h"

DamageBoost::DamageBoost() : Module("DamageBoost", "Boosts your speed when you take damage.")
{
	this->boostAmount = this->addItem<Setting>("Boost Amount", "The amount of speed that should be applied when the player takes damage.", SettingType::SLIDER_FLOAT, 0.6f, 0.0f, 1.5f);
}
void DamageBoost::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ActorMotionEvent, &DamageBoost::onActorMotionEvent>(this);
}
void DamageBoost::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<ActorMotionEvent, &DamageBoost::onActorMotionEvent>(this);
}
void DamageBoost::onTick() {

}

void DamageBoost::onActorMotionEvent(ActorMotionEvent& event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;

	float boostVal = this->boostAmount->getValueAs<float>();
	Vector2<float> lookingVec = player->rotationComponent->Get();
	Vector3<float>* newMotion = event.getMotion();
	newMotion->x = cos((lookingVec.y + 90.0f) * (PI / 180.0f)) * boostVal;
	newMotion->z = sin((lookingVec.y + 90.0f) * (PI / 180.0f)) * boostVal;
	event.setMotion(*newMotion);
}