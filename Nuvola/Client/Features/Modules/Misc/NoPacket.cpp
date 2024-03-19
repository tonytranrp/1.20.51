#include "NoPacket.h"

#include "../../../Events/EventDispatcher.h"
#include "../../../Events/PacketEvent.h"
#include "../../../SystemManager.h"

#include <xorstr.hpp>

NoPacket::NoPacket() : Module(xorstr_("NoPacket"), xorstr_("Prevents packets from being sent to the server. NOTE: This is not blink, the packets will never be sent.")) {};

void NoPacket::onPacketEvent(PacketEvent& event) {
	event.cancel();
}

void NoPacket::onEnable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<PacketEvent, &NoPacket::onPacketEvent>(this);
};

void NoPacket::onDisable() {
	auto dispatcher = systems->getEventDispatcher();
	dispatcher->deafen<PacketEvent, &NoPacket::onPacketEvent>(this);
}