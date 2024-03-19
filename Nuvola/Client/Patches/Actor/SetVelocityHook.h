#pragma once

#include "../../Bridge/Mob.h"
#include "../IPatch.h"

class SetVelocityHook : public IPatch {
	static void __fastcall SetVelocityCallback_1_20_0(Actor* actor, Vector3<float>* velocity);
public:
	static inline uintptr_t funcOriginal = 0;
	SetVelocityHook();
	bool apply() override;
};