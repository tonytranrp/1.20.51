#pragma once

#include "Components/AttributesComponent.h"
#include "Actor.h"
#include "Macro.h"

#include <bitset>

struct Mob : public Actor {
	/* Fields */
	DEFINE_MEMBER(float, mSwingProgress, OffsetTable::Mob_mSwingProgress);
	DEFINE_MEMBER(bool, mIsSwinging, OffsetTable::Mob_mIsSwinging);
	DEFINE_MEMBER(int32_t, mSwingTicks, OffsetTable::Mob_mSwingTicks);
	DEFINE_MEMBER(class MobHurtTimeComponent*, mHurtTimeComponent, OffsetTable::Mob_mHurtTimeComponent);

	/* Virtuals */
	//virtual ~Mob();

	void setSprinting(bool sprinting)
	{
		vftCall<171, void, Mob*, bool>(this, sprinting);
	}
};