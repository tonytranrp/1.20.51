#pragma once

#include "../IPatch.h"

class SendHook : public IPatch {
	static void __fastcall SendHookCallback_1_19_30(class LoopbackPacketSender* packetSender, class Packet* packet);
public:
	static inline uintptr_t funcOriginal = 0;
	SendHook();
	bool apply() override;
};
