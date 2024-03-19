#pragma once

#include "../Module.h"

class RenderEvent;
class PacketEvent;
class PacketDebugger : public Module {
public:
	PacketDebugger();
	void onEnable() override;
	void onDisable() override;

	void onRenderEvent(RenderEvent& event);
	void onPacketEvent(PacketEvent& event);
};