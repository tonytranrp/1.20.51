#pragma once

#include "../../Bridge/Mob.h"
#include "../IPatch.h"

class MobJumpFromGroundHook : public IPatch
{
	static void __cdecl MobJumpFromGroundCallback_1_20_12_1(class StrictEntityContext *param_1,
			AABBShapeComponent* param_2, class SynchedActorDataComponent* param_3, ActorRotationComponent* param_4,
			class MobEffectsComponent* param_5, class JumpControlComponent* param_6, class MobJumpComponent* param_7,
			StateVectorComponent* param_8, class EntityModifierT* param_9, class IConstBlockSource* param_10,
			bool param_11);
public:
	static inline uintptr_t funcOriginal = 0;
	MobJumpFromGroundHook();
	bool apply() override;
};