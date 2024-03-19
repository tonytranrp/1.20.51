#include "ServerRotations.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/IntOption.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/MovePlayerPacket.h"
#include "../../../Bridge/Options.h"
#include "../../../Bridge/PlayerAuthInputPacket.h"

#include "../../../Events/ActorRotateEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/PacketEvent.h"

ServerRotations::ServerRotations() : Module("ServerRotations", "Renders your player's rotations based on the packets sent to the server instead of calculating them on the client.")
{
	this->setEnabled(true);
}

void ServerRotations::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ActorRotateEvent, &ServerRotations::onActorRotateEvent>(this);
	dispatcher->listen<PacketEvent, &ServerRotations::onPacketEvent, nes::Priority::Last>(this);
}

void ServerRotations::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<ActorRotateEvent, &ServerRotations::onActorRotateEvent>(this);
	dispatcher->deafen<PacketEvent, &ServerRotations::onPacketEvent>(this);
}

void ServerRotations::setRotation(const Vector2<float> &rotation) {
	this->oldLastRotation = this->lastRotation;
	this->lastRotation = rotation;
}

void ServerRotations::onActorRotateEvent(ActorRotateEvent &event) {
	const auto* ci = Bridge::getClientInstance();
	CHECK(ci);
	if (Options* options = ci->getOptions()) {
		auto* cameraState = options->getOption<IntOption, Options::OptionID::game_thridperson>();
		if(cameraState->value == 0)
		{
			return;
		}
	}

	const auto* player = ci->getClientPlayer();
	CHECK(player);
	if(event.getActor() == player && event.getRotationType() == RotationType::RENDER)
	{
		event.setRotation(this->lastRotation);
		event.setOldRotation(this->oldLastRotation);
	}
}

void ServerRotations::onPacketEvent(PacketEvent &event) {
	if(event.getPacket()->getId() == PacketID::PlayerAuthInputPacket)
	{
		auto* paip = reinterpret_cast<PlayerAuthInputPacket*>(event.getPacket());
		this->setRotation(paip->rotation);
	}
	else if (event.getPacket()->getId() == PacketID::MovePlayerPacket)
	{
		auto* mpp = reinterpret_cast<MovePlayerPacket*>(event.getPacket());
		this->setRotation(mpp->rotation);
	}
}