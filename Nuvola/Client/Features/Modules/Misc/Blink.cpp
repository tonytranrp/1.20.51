#include "Blink.h"

#include "../../../Bridge/LoopbackPacketSender.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"
#include "../../../Bridge/MovePlayerPacket.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/RakPeerRunUpdateCycleEvent.h"

Blink::Blink() : Module("Blink","Prevents packets from being sent to the server while enabled. The packets will be sent when the module is disabled.")
{
	this->delay = this->addItem<Setting>("Delay MS", "The packet delay (in ms).", SettingType::SLIDER_FLOAT, 500.f, 0.f, 10000.f);
	this->delayEnabled = this->addItem<Setting>("Packet Delay", "If enabled, packets will be sent every delay interval.", SettingType::TOGGLE, false, false, true);
}

void Blink::onEnable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<RakPeerRunUpdateCycleEvent, &Blink::onRakPeerRunUpdateCycleEvent>(this);
}

void Blink::onDisable()
{
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<RakPeerRunUpdateCycleEvent, &Blink::onRakPeerRunUpdateCycleEvent>(this);
}

void Blink::onRakPeerRunUpdateCycleEvent(RakPeerRunUpdateCycleEvent& event)
{
	if (!this->delayEnabled->getValueAs<bool>())
	{
		event.setCancelled(true);
		return;
	}

	auto currentMS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	if (this->lastSentTime == 0.f) 
	{
		this->lastSentTime = currentMS;

		return;
	}

	if ((currentMS - this->lastSentTime) < this->delay->getValueAs<float>()
		&& this->delayEnabled->getValueAs<bool>())
	{
		this->lastSentTime = currentMS;
		event.setCancelled(false);
		return;
	}
	else {
		event.setCancelled(true);
	}
}