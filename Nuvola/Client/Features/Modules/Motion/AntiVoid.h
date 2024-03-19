#pragma once

#include "../Module.h"

class PacketEvent;
class AntiVoid : public Module
{
public:
	AntiVoid();

	virtual void onEnable();
	virtual void onDisable();

	void onPacketEvent(PacketEvent&);
};