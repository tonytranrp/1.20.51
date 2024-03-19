#include "HighJump.h"

#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/ClientInstance.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/MobQueryJumpPowerEvent.h"

HighJump::HighJump() : Module("HighJump", "Changes the player's jump velocity")
{
	this->jumpPower = this->addItem<Setting>("Jump Power", "How powerful your jump is.", SettingType::SLIDER_FLOAT, 1.0f, 0.0f, 5.0f);
}

void HighJump::onEnable() 
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<MobQueryJumpPowerEvent, &HighJump::onMobQueryJumpPowerEvent>(this);
}

void HighJump::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<MobQueryJumpPowerEvent, &HighJump::onMobQueryJumpPowerEvent>(this);
}

void HighJump::onMobQueryJumpPowerEvent(MobQueryJumpPowerEvent &event)
{
	ClientInstance* ci = Bridge::getClientInstance();
	if (ci == nullptr) return;
	LocalPlayer* player = ci->getClientPlayer();
	if (player == nullptr) return;
	if(event.getMob() != player) return;

	event.setPower(this->jumpPower->getValueAs<float>());
}
