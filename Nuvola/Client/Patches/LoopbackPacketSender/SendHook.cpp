#include  "SendHook.h"

#include "../../Bridge/LoopbackPacketSender.h"
#include "../../Bridge/Packet.h"
#include "../../Bridge/TextPacket.h"

#include "../../Events/ChatEvent.h"
#include "../../Events/EventDispatcher.h"
#include "../../Events/PacketEvent.h"
#include "../../SystemManager.h"

void __fastcall SendHook::SendHookCallback_1_19_30(LoopbackPacketSender* packetSender, Packet* packet) {

	auto event = nes::make_holder<PacketEvent>(packet);
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->trigger(event);
	if (packet->getId() == PacketID::TextPacket) {
		auto* textPacket = (TextPacket*)packet;
		auto chatEvent = nes::make_holder<ChatEvent>(textPacket->text);
		dispatcher->trigger(chatEvent);
		if (chatEvent->isCancelled()) {
			return;
		}
	}
	if(event->isCancelled()) {
		return;
	}

	PLH::FnCast(funcOriginal, &SendHookCallback_1_19_30)(packetSender, packet);
}

SendHook::SendHook() : IPatch::IPatch("LoopbackPacketSender::send") {
	this->addSignature(SigTable::LoopbackPacketSender_send);
}

bool SendHook::apply() {
	return this->autoPatch(&SendHookCallback_1_19_30, &funcOriginal);
}