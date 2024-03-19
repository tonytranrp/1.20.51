#pragma once

#include "Event.h"
#include "Cancellable.h"
#include "../Bridge/Packet.h"

class PacketEvent : public Event, public Cancellable {
	Packet* packet;
public:
	PacketEvent(Packet* packet) : Event() {
		this->packet = packet;
	};

	Packet* getPacket() {
		return this->packet;
	}
};