#pragma once

#include <Math/Math.h>

struct LoopbackPacketSender {
	/* Fields */
	/* Virtuals */
	virtual void virt_pad_0() {};
	virtual void send(class Packet* packet) {};
	virtual void sendToServer(class Packet* packet) {};
	/* Functions */
};