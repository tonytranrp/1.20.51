#pragma once

#include "../Module.h"

class PacketEvent;
class NoPacket : public Module {
public:
	NoPacket();
	void onEnable() override;
	void onDisable() override;
	void onPacketEvent(PacketEvent& event);
};
