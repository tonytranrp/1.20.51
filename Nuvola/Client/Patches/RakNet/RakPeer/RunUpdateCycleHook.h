#pragma once

#include "../../IPatch.h"
#include "../../../Bridge/RakNet/RakPeer.h"
#include "../../../Bridge/LocalPlayer.h"

class RunUpdateCycleHook : public IPatch {
	static bool __fastcall RunUpdateCycleHookCallback_1_20_0(RakNet::RakPeer* peer, void* bitStream);
public:
	static inline uintptr_t funcOriginal = 0;
	RunUpdateCycleHook();
	bool apply() override;
};